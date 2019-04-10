# Λ^
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

### eval-appl

Applicative β-normalization of lambda terms is performed using `eval-appl`.

```
> eval-appl ((^ x (x y)) z)
(z y)
```

### beta-redex?

To check if a term is a β-redex use `redex?`.

```
> redex? ((^ x x) z)
true
```

### normal-form?

To check if a term is in β-normal form use `normal-form?`.

```
> normal-form? (x ((^ y y) z))
false
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
