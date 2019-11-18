# TypingML4

![](https://github.com/darshimo/TypingML4/blob/images/demo.gif)

## Overview

An interpreter for subset of OCaml.

Given an expression, it does lexing and parsing, then outputs the derivation tree of typing judgement.

## Installation

```sh
$ cd TypingML4
$ make
```

## Usage

- Execute typingML4 without arguments and type expression.

  ```sh
  $ ./typingML4
  input expression within 500 characters (end with ;)
  > [expression];
  ```

- Give file path as command line argument.

  ```sh
  $ ./typingML4 path/to/file
  ```

  Runing `make test`, you can get the answers for sample problems.

## Derivation System

### Syntax

<div align="center">
<img src="https://github.com/darshimo/TypingML4/blob/images/syntax.png" width="70%">
</div>

### Derivation Rules

<img src="https://github.com/darshimo/TypingML4/blob/images/rules.png">

## Example

```
$ ./typingML4
input expression within 500 characters (end with ;)
> let k = fun x -> fun y -> x in k (1 :: []) 3 : int list;

|- let k = fun x -> fun y -> x in k (1 :: []) 3 : int list by T-Let {
  |- fun x -> fun y -> x : int list -> int -> int list by T-Fun {
    x : int list |- fun y -> x : int -> int list by T-Fun {
      x : int list, y : int |- x : int list by T-Var {}
    }
  };
  k : int list -> int -> int list |- k (1 :: []) 3 : int list by T-App {
    k : int list -> int -> int list |- k (1 :: []) : int -> int list by T-App {
      k : int list -> int -> int list |- k : int list -> int -> int list by T-Var {};
      k : int list -> int -> int list |- 1 :: [] : int list by T-Cons {
        k : int list -> int -> int list |- 1 : int by T-Int {};
        k : int list -> int -> int list |- [] : int list by T-Nil {}
      }
    };
    k : int list -> int -> int list |- 3 : int by T-Int {}
  }
}
```

## Requirement

gcc 8.3.0
