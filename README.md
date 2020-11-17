# quine-mccluskey-petrick
C++ implementation of Quine-McCluskey and Petrick's method

# How to use
inputs should be written to a .txt file with following format<br>
**inputs,minterms,dontcares,** 

Examples
```
A-B-C-D,0-1-3-7-8-9-11-15,                                  (without dontcares)
A-B-C-D,2-3-7-9-11-13,1-10-15,                              (with dontcares)
```
**Note**: Please, keep a comma at the end of a line. One expression to reduce per line. "dontcares" are optional.

# Example use case
CLI

```bash
$ make
$ ./bin/qmp input.txt
```

input.txt
```
A-B-C-D,0-1-3-7-8-9-11-15,
A-B-C-D,0-2-5-6-7-8-10-12-13-14-15, 
A-B-C-D,0-2-3-4-5-6-7-8-9-10-11-12-13,
A-B-C-D,2-3-7-9-11-13,1-10-15,
```
stdout (CLI)
```
Solution: B'C' + CD

Solution: BC + AD' + B'D' + BD

Solution: BC' + AB' + B'D' + A'C

Solution: B'C + CD + AD
```
