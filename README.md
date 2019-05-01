# Λ^

[![Build Status](https://travis-ci.org/sgatev/lhat.svg?branch=master)](https://travis-ci.org/sgatev/lhat)

An interpreter for lambda calculus.

## Requires

* C++ 17 compatible compiler
* [Bazel](https://bazel.build)

## Build

To build all targets execute

```bash
$ bazel build ...
```

## Test

To test all targets execute

```bash
$ bazel test ...
```

## Run

To run the lambda calculus interpreter execute

```bash
$ bazel run //lhat/repl:main
```

## Commands

### alpha-equiv?

To check if two terms are α-equivalent use `alpha-equiv?`.

```
> alpha-equiv? ((^ x x) z) ((^ y y) z)
true
```

### beta-redex?

To check if a term is a β-redex use `beta-redex?`.

```
> beta-redex? ((^ x x) z)
true
```

### beta-normal?

To check if a term is in β-normal form use `beta-normal?`.

```
> beta-normal? (x ((^ y y) z))
false
```

### head-normal?

To check if a term is in head normal form use `head-normal?`.

```
> head-normal? (^ x (^ y ((^ z (y z)) x)))
false
```

### beta-reduce

β-reduction of lambda terms is performed using `beta-reduce`.

```
> beta-reduce ((^ x x) ((^ y y) z))
((^ a a) z)
```

### beta-eval-appl

Applicative β-normalization of lambda terms is performed using `beta-eval-appl`.

```
> beta-eval-appl ((^ x (x y)) z)
(z y)
```

### beta-eval-normal

Normal β-normalization of lambda terms is performed using `beta-eval-normal`.

```
> beta-eval-normal ((^ x y) ((^ x (x x)) (^ x (x x))))
y
```

### eta-reduce

η-reduction of lambda terms is performed using `eta-reduce`.

```
> eta-reduce (^ x ((^ y z) x))
(^ a z)
```

### eta-eval

η-normalization of lambda terms is performed using `eta-eval`.

```
> eta-eval (^ v ((w (^ x ((^ y z) x))) v))
(w (^ a z))
```

### infer-type

To infer the type of a term use `infer-type`.

```
> infer-type (^ x (^ y (x y)))
(T4 -> T5) -> T4 -> T5
```

### def

The interpreter supports definition of constants via the `def` command. While
reducing a lambda term it automatically substitutes the constants present in the
term for their predefined values. To reference a constant in a term prefix its name
by `'`.

```
> def K (^ x (^ y x))
> def I (^ x x)
> def K* ('K 'I)
> eval-appl ('K z)
(^ a z)
> eval-appl ('K* z)
(^ a a)
```

## Flags

The behavior of the interpreter can be controlled using the following command-line
flags.

### --exec

The interpreter could execute a list of commands specified in a file before
becoming interactive. Pass the path to the file as an argument to the `--exec`
flag. Multiple `--exec` flags could be provided in a single run of the interpreter.

```
$ bazel run //lhat/repl:main -- --exec /git/lhat/examples/combinators
> eval-appl ('K z)
(^ a z)
```
