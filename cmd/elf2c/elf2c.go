/*
Copyright 2018 Harvey OS Team

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

package main

import (
	"bytes"
	"debug/elf"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"math"
	"os"
	"path"
)

var dry = flag.Bool("dryrun", true, "don't really do it")

func gencode(w io.Writer, n, t string, m []byte, start, end uint64) {
	fmt.Fprintf(os.Stderr, "Write %v %v start %v end %v\n", n, t, start, end)
	fmt.Fprintf(w, "u64 %v_%v_start = %#x;\n", n, t, start)
	fmt.Fprintf(w, "u64 %v_%v_end = %#x;\n", n, t, end)
	fmt.Fprintf(w, "u64 %v_%v_len = %#x;\n", n, t, end-start)
	fmt.Fprintf(w, "u8 %v_%v_out[] = {\n", n, t)
	for i := uint64(start); i < end; i += 16 {
		for j := uint64(0); i+j < end && j < 16; j++ {
			fmt.Fprintf(w, "%#02x, ", m[j+i])
		}
		fmt.Fprintf(w, "\n")
	}
	fmt.Fprintf(w, "};\n")
}
func main() {
	flag.Parse()
	a := flag.Args()
	w := &bytes.Buffer{}
	for _, n := range a[1:] {
		f, err := elf.Open(n)
		if err != nil {
			fmt.Printf("%v %v\n", n, err)
			continue
		}
		var dataend, codeend, end uint64
		var datastart, codestart, start uint64
		datastart, codestart, start = math.MaxUint64, math.MaxUint64, math.MaxUint64
		mem := []byte{}
		for _, v := range f.Progs {
			if v.Type != elf.PT_LOAD {
				continue
			}
			fmt.Fprintf(os.Stderr, "processing %v\n", v)
			// MUST alignt to 2M page boundary.
			// then MUST allocate a []byte that
			// is the right size. And MUST
			// see if by some off chance it
			// joins to a pre-existing segment.
			// It's easier than it seems. We produce ONE text
			// array and ONE data array. So it's a matter of creating
			// a virtual memory space with an assumed starting point of
			// 0x200000, and filling it. We just grow that as needed.

			curstart := v.Vaddr & ^uint64(0xfff) // 0x1fffff)
			curend := v.Vaddr + v.Memsz
			fmt.Fprintf(os.Stderr, "s %x e %x\n", curstart, curend)
			if curend > end {
				nmem := make([]byte, curend)
				copy(nmem, mem)
				mem = nmem
			}
			if curstart < start {
				start = curstart
			}

			if v.Flags&elf.PF_X == elf.PF_X {
				if curstart < codestart {
					codestart = curstart
				}
				if curend > codeend {
					codeend = curend
				}
				fmt.Fprintf(os.Stderr, "code s %v e %v\n", codestart, codeend)
			} else {
				if curstart < datastart {
					datastart = curstart
				}
				if curend > dataend {
					dataend = curend
				}
				fmt.Fprintf(os.Stderr, "data s %v e %v\n", datastart, dataend)
			}
			for i := uint64(0); i < v.Filesz; i++ {
				if amt, err := v.ReadAt(mem[v.Vaddr+i:], int64(i)); err != nil && err != io.EOF {
					fmt.Fprintf(os.Stderr, "%v: %v\n", amt, err)
					os.Exit(1)
				} else if amt == 0 {
					if i < v.Filesz {
						fmt.Fprintf(os.Stderr, "%v: Short read: %v of %v\n", v, i, v.Filesz)
						os.Exit(1)
					}
					break
				} else {
					i = i + uint64(amt)
					fmt.Fprintf(os.Stderr, "i now %v\n", i)
				}
			}
			fmt.Fprintf(os.Stderr, "Processed %v\n", v)
		}
		fmt.Fprintf(os.Stderr, "gencode\n")
		_, file := path.Split(n)
		fmt.Fprintf(w, "uintptr_t %v_main = %v;\n", n, f.Entry)
		gencode(w, file, "code", mem, codestart, codeend)
		gencode(w, file, "data", mem, datastart, dataend)
	}
	if err := ioutil.WriteFile(a[0], w.Bytes(), 0444); err != nil {
		fmt.Fprintf(os.Stderr, "elf2c: write %s failed: %v\n", a[0], err)
	}

}