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

The interpreter outputs the β-reduction of the input lambda term.

```
> ((^ x (x y)) z)
(z y)
```

## Function

### Defs

The interpreter supports definition of constants via the `def` command. While
reducing a lambda term it automatically substitutes the constants present in the
term for their predefined values. To reference a constant in a term prefix its name
by `$`.

```
> def K (^ x (^ y x))
> def I (^ x x)
> def K* ($K $I)
> ($K z)
(^ a z)
> ($K* z)
(^ a a)
```
