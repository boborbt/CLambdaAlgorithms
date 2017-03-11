# TODO

- Create saner Array interface: it will always store pointers. The current
  implementation need to be renamed and kept to serve particular cases.

- Change every single typedef to not include the pointer to the structure.
  e.g., `typedef struct _Array* Array` should be `typedef struct _Array Array`
