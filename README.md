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

### eval-appl

Applicative β-normalization of lambda terms is performed using `eval-appl`.

```
> eval-appl ((^ x (x y)) z)
(z y)
```

### eval-normal

Normal β-normalization of lambda terms is performed using `eval-normal`.

```
> eval-normal ((^ x y) ((^ x (x x)) (^ x (x x))))
y
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
