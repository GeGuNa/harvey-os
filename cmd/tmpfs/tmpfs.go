package main

import (
	"bytes"
	"flag"
	"fmt"
	"log"
	"net"
	"sync"

	"github.com/Harvey-OS/go/internal/tmpfs"
	"github.com/Harvey-OS/ninep/pkg/debugfs"
	"github.com/Harvey-OS/ninep/protocol"
)

var (
	networktype = flag.String("ntype", "tcp4", "Default network type")
	netaddr     = flag.String("addr", ":5641", "Network address")
	debug       = flag.Int("debug", 0, "Print debug messages")
)

type fileServer struct {
	Versioned bool

	archive    *tmpfs.Archive
	filesMutex sync.Mutex
	files      map[protocol.FID]*FidEntry
	ioUnit     protocol.MaxSize
}

// FidEntry wraps an Entry with the instance data required for a fid reference
type FidEntry struct {
	tmpfs.Entry

	// We can't know how big a serialized dentry is until we serialize it.
	// At that point it might be too big. We save it here if that happens,
	// and on the next directory read we start with that.
	oflow []byte

	// Index of next child to return when reading a directory
	nextChildIdx int
}

func newFidEntry(entry tmpfs.Entry) *FidEntry {
	return &FidEntry{Entry: entry, oflow: nil, nextChildIdx: -1}
}

func (fs *fileServer) Rversion(msize protocol.MaxSize, version string) (protocol.MaxSize, string, error) {
	if version != "9P2000" {
		return 0, "", fmt.Errorf("%v not supported; only 9P2000", version)
	}
	return msize, version, nil
}

// TODO handle aname?
func (fs *fileServer) Rattach(fid protocol.FID, afid protocol.FID, uname string, aname string) (protocol.QID, error) {
	if afid != protocol.NOFID {
		return protocol.QID{}, fmt.Errorf("We don't do auth attach")
	}

	// There should be no .. or other such junk in the Aname. Clean it up anyway.
	/*aname = path.Join("/", aname)
	st, err := os.Stat(aname)
	if err != nil {
		return protocol.QID{}, err
	}*/

	root := fs.archive.Root()
	fs.filesMutex.Lock()
	fs.files[fid] = newFidEntry(root)
	fs.filesMutex.Unlock()

	return root.Qid(), nil
}

func (fs *fileServer) Rflush(o protocol.Tag) error {
	return nil
}

func (fs *fileServer) Rwalk(fid protocol.FID, newfid protocol.FID, paths []string) ([]protocol.QID, error) {
	fmt.Printf("walk paths: %v\n", paths)
	// Lookup the parent fid
	parentEntry, err := fs.getFile(fid)
	if err != nil {
		return nil, fmt.Errorf("does not exist")
	}

	if len(paths) == 0 {
		fmt.Printf("walk len(paths): %v\n", len(paths))
		// Clone fid - point to same entry
		fs.filesMutex.Lock()
		defer fs.filesMutex.Unlock()
		_, ok := fs.files[newfid]
		if ok {
			return nil, fmt.Errorf("FID in use: clone walk, fid %d newfid %d", fid, newfid)
		}

		fs.files[newfid] = newFidEntry(parentEntry.Entry)
		return []protocol.QID{}, nil
	}

	walkQids := make([]protocol.QID, len(paths))

	var i int
	var pathcmp string
	currEntry := parentEntry.Entry
	fmt.Printf("walk currEntry: %v\n", currEntry.FullName())
	for i, pathcmp = range paths {
		fmt.Printf("walk i: %v pathcmp: %v\n", i, pathcmp)

		var ok bool
		var dir *tmpfs.Directory
		if dir, ok = currEntry.(*tmpfs.Directory); ok {
			currEntry, ok = dir.ChildByName(pathcmp)
			if !ok {
				fmt.Printf("walk currEntry (not ok)\n")

				// From the RFC: If the first element cannot be walked for any
				// reason, Rerror is returned. Otherwise, the walk will return an
				// Rwalk message containing nwqid qids corresponding, in order, to
				// the files that are visited by the nwqid successful elementwise
				// walks; nwqid is therefore either nwname or the index of the
				// first elementwise walk that failed. The value of nwqid cannot be
				// zero unless nwname is zero. Also, nwqid will always be less than
				// or equal to nwname. Only if it is equal, however, will newfid be
				// affected, in which case newfid will represent the file reached
				// by the final elementwise walk requested in the message.
				//
				// to sum up: if any walks have succeeded, you return the QIDS for
				// one more than the last successful walk
				if i == 0 {
					return nil, fmt.Errorf("file does not exist")
				}
				// we only get here if i is > 0 and less than nwname,
				// so the i should be safe.
				return walkQids[:i], nil
			}
		}
		fmt.Printf("walk currEntry (ok): %v\n", currEntry.FullName())
		walkQids[i] = currEntry.Qid()
	}
	fn := currEntry.FullName()
	fmt.Println(fn)

	fs.filesMutex.Lock()
	defer fs.filesMutex.Unlock()
	// this is quite unlikely, which is why we don't bother checking for it first.
	if fid != newfid {
		if _, ok := fs.files[newfid]; ok {
			return nil, fmt.Errorf("FID in use: walk to %v, fid %v, newfid %v", paths, fid, newfid)
		}
	}
	fs.files[newfid] = newFidEntry(currEntry)
	return walkQids, nil
}

func (fs *fileServer) Ropen(fid protocol.FID, mode protocol.Mode) (protocol.QID, protocol.MaxSize, error) {
	if mode&(protocol.OWRITE|protocol.ORDWR|protocol.OTRUNC|protocol.ORCLOSE|protocol.OAPPEND) != 0 {
		return protocol.QID{}, 0, fmt.Errorf("filesystem is read-only")
	}

	// Lookup the parent fid
	f, err := fs.getFile(fid)
	if err != nil {
		return protocol.QID{}, 0, fmt.Errorf("does not exist")
	}

	// TODO Check executable

	return f.Qid(), fs.ioUnit, nil
}

func (fs *fileServer) Rcreate(fid protocol.FID, name string, perm protocol.Perm, mode protocol.Mode) (protocol.QID, protocol.MaxSize, error) {
	return protocol.QID{}, 0, fmt.Errorf("filesystem is read-only")
}

func (fs *fileServer) Rclunk(fid protocol.FID) error {
	_, err := fs.clunk(fid)
	return err
}

func (fs *fileServer) Rstat(fid protocol.FID) ([]byte, error) {
	f, err := fs.getFile(fid)
	if err != nil {
		return []byte{}, err
	}
	d := f.P9Dir()
	var b bytes.Buffer
	protocol.Marshaldir(&b, *d)
	return b.Bytes(), nil
}

func (fs *fileServer) Rwstat(fid protocol.FID, b []byte) error {
	return fmt.Errorf("filesystem is read-only")
}

func (fs *fileServer) Rremove(fid protocol.FID) error {
	return fmt.Errorf("filesystem is read-only")
}

func (fs *fileServer) Rread(fid protocol.FID, o protocol.Offset, c protocol.Count) ([]byte, error) {
	f, err := fs.getFile(fid)
	if err != nil {
		return nil, err
	}

	if dir, ok := f.Entry.(*tmpfs.Directory); ok {
		fmt.Printf("read dir\n")
		if o == 0 {
			f.oflow = nil
		}

		// We make the assumption that they can always fit at least one
		// directory entry into a read. If that assumption does not hold
		// so many things are broken that we can't fix them here.
		// But we'll drop out of the loop below having returned nothing
		// anyway.
		b := bytes.NewBuffer(f.oflow)
		f.oflow = nil
		pos := 0

		for {
			if b.Len() > int(c) {
				f.oflow = b.Bytes()[pos:]
				return b.Bytes()[:pos], nil
			}

			f.nextChildIdx++
			pos += b.Len()

			if f.nextChildIdx >= dir.NumChildren() {
				return b.Bytes(), nil
			}
			d9p := dir.Child(f.nextChildIdx).P9Dir()
			protocol.Marshaldir(b, *d9p)

			// Seen on linux clients: sometimes the math is wrong and
			// they end up asking for the last element with not enough data.
			// Linux bug or bug with this server? Not sure yet.
			if b.Len() > int(c) {
				log.Printf("Warning: Server bug? %v, need %d bytes;count is %d: skipping", d9p, b.Len(), c)
				return nil, nil
			}
			// TODO handle more than one entry at a time
			// We're not quite doing the array right.
			// What does work is returning one thing so, for now, do that.
			return b.Bytes(), nil
		}

	} else if file, ok := f.Entry.(*tmpfs.File); ok {
		fmt.Printf("read file\n")

		// N.B. even if they ask for 0 bytes on some file systems it is important to pass
		// through a zero byte read (not Unix, of course).
		/*b := make([]byte, c)
		n, err := file.Data().ReadAt(b, int64(o))
		if err != nil && err != io.EOF {
			return nil, err
		}
		return b[:n], nil*/

		// TODO validate
		end := uint64(o) + uint64(c)
		return file.Data()[o:end], nil
	}
	log.Fatalf("Unrecognised FidEntry")
	return nil, nil
}

func (fs *fileServer) Rwrite(fid protocol.FID, o protocol.Offset, b []byte) (protocol.Count, error) {
	return -1, fmt.Errorf("filesystem is read-only")
}

func (fs *fileServer) getFile(fid protocol.FID) (*FidEntry, error) {
	fs.filesMutex.Lock()
	defer fs.filesMutex.Unlock()

	f, ok := fs.files[fid]
	if !ok {
		return nil, fmt.Errorf("does not exist")
	}
	return f, nil
}

func (fs *fileServer) clunk(fid protocol.FID) (tmpfs.Entry, error) {
	fs.filesMutex.Lock()
	defer fs.filesMutex.Unlock()
	f, ok := fs.files[fid]
	if !ok {
		return nil, fmt.Errorf("does not exist")
	}
	delete(fs.files, fid)

	return f, nil
}

func newTmpfs(arch *tmpfs.Archive, opts ...protocol.ListenerOpt) (*protocol.Listener, error) {
	if arch == nil {
		return nil, fmt.Errorf("No archive")
	}

	nsCreator := func() protocol.NineServer {
		fs := &fileServer{archive: arch}
		fs.files = make(map[protocol.FID]*FidEntry)
		fs.ioUnit = 8192

		var ns protocol.NineServer = fs
		if *debug != 0 {
			ns = &debugfs.DebugFileServer{FileServer: fs}
		}
		return ns
	}

	l, err := protocol.NewListener(nsCreator, opts...)
	if err != nil {
		return nil, err
	}
	return l, nil
}

func main() {
	flag.Parse()

	// TODO replace with loading from cmd line
	buf := tmpfs.CreateTestImageTemp()
	arch := tmpfs.ReadImage(buf)

	// Bind and listen on the socket.
	listener, err := net.Listen(*networktype, *netaddr)
	if err != nil {
		log.Fatalf("Listen failed: %v", err)
	}

	ufslistener, err := newTmpfs(arch, func(l *protocol.Listener) error {
		l.Trace = nil // log.Printf
		return nil
	})

	if err := ufslistener.Serve(listener); err != nil {
		log.Fatal(err)
	}
}
