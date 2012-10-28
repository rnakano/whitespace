#ifndef _COMMAND_H_
#define _COMMAND_H_

enum Operation {PUSH, DUPLICATE, COPY, SWAP, DISCARD, SLIDE,
                ADD, SUB, MUL, DIV, MOD,
                STORE, RETRIEVE,
                MARK, CALL, JUMP, JUMPIFZERO, JUMPIFNEGATIVE, RETURN, END,
                PUTC, PUTN, READC, READN,
                /* Optimize operation */
                PUSH_ADD, NOP
};

struct Command_T {
  enum Operation op;
  int param;
};

typedef struct Command_T Command;

#endif /* _COMMAND_H_ */
