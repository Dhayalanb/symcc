// This file is part of SymCC.
//
// SymCC is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// SymCC is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// SymCC. If not, see <https://www.gnu.org/licenses/>.

// RUN: /bin/echo -ne "\x05\x01\x00\x00aaaa" > %T/%basename_t.input
// RUN: %symcc -O2 %s -o %t
// RUN: env SYMCC_SYMBOLIC_BYTES=1  SYMCC_INPUT_FILE=%T/%basename_t.input %t %T/%basename_t.input 2>&1 | %filecheck %s

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  //
  // Read from the input file using Unix primitives.
  //

  // ANY-NOT: Warning
  int fd = open(argv[1], O_RDONLY);
  if (fd < 0) {
    perror("failed to open the input file");
    return -1;
  }

  char input_0;
  if (read(fd, &input_0, sizeof(input_0)) != 1) {
    perror("failed to read from the input file");
    return -1;
  }

  char input_1;
  if (read(fd, &input_1, sizeof(input_1)) != 1) {
    perror("failed to read from the input file");
    return -1;
  }

  // SIMPLE-NOT: Trying to solve
  // QSYM-NOT: SMT
  // ANY: This may be the answer.
  if (input_0 == 5)
    printf("This may be the answer.\n");
  else
    printf("Not sure this is correct...\n");

  // SIMPLE: Trying to solve
  // SIMPLE: Found diverging input
  // QSYM-COUNT-2: SMT
  // QSYM: New testcase
  // ANY: All is good.
  if (input_1 == 1)
    printf("All is good.\n");
  else
    printf("Not sure this is correct...\n");
  
  return 0;
}
