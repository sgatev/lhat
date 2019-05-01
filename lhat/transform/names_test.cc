#include "lhat/transform/names.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace lhat {
namespace transform {
namespace {
using ::testing::IsEmpty;
using ::testing::NotNull;
using ::testing::UnorderedElementsAre;

TEST(NameContext, AddAndGetNames) {
  NameContext nctx;

  nctx.AddName("foo");
  nctx.AddName("bar");

  EXPECT_THAT(nctx.Names(), UnorderedElementsAre("foo", "bar"));

  EXPECT_EQ(nctx.GetIndexForName("foo"), 0);
  EXPECT_EQ(nctx.GetNameForIndex(0), "foo");

  EXPECT_EQ(nctx.GetIndexForName("bar"), 1);
  EXPECT_EQ(nctx.GetNameForIndex(1), "bar");
}

TEST(RemoveNames, Var) {
  NameContext nctx;

  const named::Term named_term = named::Var("x");
  const nameless::Term nameless_term = RemoveNames(named_term, &nctx);

  const nameless::Var* var = nameless_term.Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);

  EXPECT_THAT(nctx.Names(), UnorderedElementsAre("x"));
  EXPECT_EQ(nctx.GetIndexForName("x"), 0);
}

TEST(RemoveNames, AbstBound) {
  NameContext nctx;

  const named::Term named_term = named::Abst("x", named::Var("x"));
  const nameless::Term nameless_term = RemoveNames(named_term, &nctx);

  const nameless::Abst* abst = nameless_term.Get<nameless::Abst>();
  ASSERT_THAT(abst, NotNull());

  const nameless::Var* var = abst->Body().Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), -1);

  EXPECT_THAT(nctx.Names(), IsEmpty());
}

TEST(RemoveNames, AbstFree) {
  NameContext nctx;

  const named::Term named_term = named::Abst("x", named::Var("y"));
  const nameless::Term nameless_term = RemoveNames(named_term, &nctx);

  const nameless::Abst* abst = nameless_term.Get<nameless::Abst>();
  ASSERT_THAT(abst, NotNull());

  const nameless::Var* var = abst->Body().Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), 0);

  EXPECT_THAT(nctx.Names(), UnorderedElementsAre("y"));
  EXPECT_EQ(nctx.GetIndexForName("y"), 0);
}

TEST(RemoveNames, Appl) {
  NameContext nctx;

  const named::Term named_term = named::Appl(named::Var("x"), named::Var("y"));
  const nameless::Term nameless_term = RemoveNames(named_term, &nctx);

  const nameless::Appl* appl = nameless_term.Get<nameless::Appl>();
  ASSERT_THAT(appl, NotNull());

  const nameless::Var* func_var = appl->Func().Get<nameless::Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Index(), 0);

  const nameless::Var* arg_var = appl->Arg().Get<nameless::Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Index(), 1);

  EXPECT_THAT(nctx.Names(), UnorderedElementsAre("x", "y"));
  EXPECT_EQ(nctx.GetIndexForName("x"), 0);
  EXPECT_EQ(nctx.GetIndexForName("y"), 1);
}

TEST(RemoveNames, Complex) {
  NameContext nctx;

  const named::Term named_term =
      named::Appl(named::Abst("x", named::Var("y")),
                  named::Appl(named::Var("u"), named::Var("v")));
  const nameless::Term nameless_term = RemoveNames(named_term, &nctx);

  const nameless::Appl* appl = nameless_term.Get<nameless::Appl>();
  ASSERT_THAT(appl, NotNull());

  const nameless::Abst* func_abst = appl->Func().Get<nameless::Abst>();
  ASSERT_THAT(func_abst, NotNull());

  const nameless::Var* abst_var = func_abst->Body().Get<nameless::Var>();
  ASSERT_THAT(abst_var, NotNull());
  EXPECT_EQ(abst_var->Index(), 0);

  const nameless::Appl* arg_appl = appl->Arg().Get<nameless::Appl>();
  ASSERT_THAT(arg_appl, NotNull());

  const nameless::Var* arg_func_var = arg_appl->Func().Get<nameless::Var>();
  ASSERT_THAT(arg_func_var, NotNull());
  EXPECT_EQ(arg_func_var->Index(), 1);

  const nameless::Var* arg_arg_var = arg_appl->Arg().Get<nameless::Var>();
  ASSERT_THAT(arg_arg_var, NotNull());
  EXPECT_EQ(arg_arg_var->Index(), 2);

  EXPECT_THAT(nctx.Names(), UnorderedElementsAre("y", "u", "v"));
  EXPECT_EQ(nctx.GetIndexForName("y"), 0);
  EXPECT_EQ(nctx.GetIndexForName("u"), 1);
  EXPECT_EQ(nctx.GetIndexForName("v"), 2);
}

TEST(RemoveNames, NestedAbstTerms) {
  NameContext nctx;

  const named::Term named_term =
      named::Abst("a", named::Abst("b", named::Abst("c", named::Var("b"))));
  const nameless::Term nameless_term = RemoveNames(named_term, &nctx);

  const nameless::Abst* abst = nameless_term.Get<nameless::Abst>();
  ASSERT_THAT(abst, NotNull());

  const nameless::Abst* body_abst = abst->Body().Get<nameless::Abst>();
  ASSERT_THAT(body_abst, NotNull());

  const nameless::Abst* body_body_abst =
      body_abst->Body().Get<nameless::Abst>();
  ASSERT_THAT(body_body_abst, NotNull());

  const nameless::Var* var = body_body_abst->Body().Get<nameless::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Index(), -2);
}

TEST(AddNames, Var) {
  const nameless::Term nameless_term = nameless::Var(1);
  NameContext free_nctx;
  const named::Term named_term = AddNames(nameless_term, &free_nctx);

  const named::Var* var = named_term.Get<named::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "a");
}

TEST(AddNames, AbstFree) {
  const nameless::Term nameless_term = nameless::Abst(nameless::Var(0));
  NameContext free_nctx;
  const named::Term named_term = AddNames(nameless_term, &free_nctx);

  const named::Abst* abst = named_term.Get<named::Abst>();
  ASSERT_THAT(abst, NotNull());
  EXPECT_EQ(abst->VarName(), "a");

  const named::Var* var = abst->Body().Get<named::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "b");
}

TEST(AddNames, AbstBound) {
  const nameless::Term nameless_term = nameless::Abst(nameless::Var(-1));
  NameContext free_nctx;
  const named::Term named_term = AddNames(nameless_term, &free_nctx);

  const named::Abst* abst = named_term.Get<named::Abst>();
  ASSERT_THAT(abst, NotNull());
  EXPECT_EQ(abst->VarName(), "a");

  const named::Var* var = abst->Body().Get<named::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "a");
}

TEST(AddNames, Appl) {
  const nameless::Term nameless_term =
      nameless::Appl(nameless::Var(0), nameless::Var(1));
  NameContext free_nctx;
  const named::Term named_term = AddNames(nameless_term, &free_nctx);

  const named::Appl* appl = named_term.Get<named::Appl>();
  ASSERT_THAT(appl, NotNull());

  const named::Var* func_var = appl->Func().Get<named::Var>();
  ASSERT_THAT(func_var, NotNull());
  EXPECT_EQ(func_var->Name(), "a");

  const named::Var* arg_var = appl->Arg().Get<named::Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "b");
}

TEST(AddNames, Complex) {
  const nameless::Term nameless_term = nameless::Abst(
      nameless::Appl(nameless::Abst(nameless::Var(-2)), nameless::Var(0)));
  NameContext free_nctx;
  const named::Term named_term = AddNames(nameless_term, &free_nctx);

  const named::Abst* abst = named_term.Get<named::Abst>();
  ASSERT_THAT(abst, NotNull());
  EXPECT_EQ(abst->VarName(), "a");

  const named::Appl* appl = abst->Body().Get<named::Appl>();
  ASSERT_THAT(appl, NotNull());

  const named::Abst* func_abst = appl->Func().Get<named::Abst>();
  ASSERT_THAT(func_abst, NotNull());
  EXPECT_EQ(func_abst->VarName(), "b");

  const named::Var* func_abst_var = func_abst->Body().Get<named::Var>();
  ASSERT_THAT(func_abst_var, NotNull());
  EXPECT_EQ(func_abst_var->Name(), "a");

  const named::Var* arg_var = appl->Arg().Get<named::Var>();
  ASSERT_THAT(arg_var, NotNull());
  EXPECT_EQ(arg_var->Name(), "c");
}

TEST(AddNames, NestedAbst) {
  const nameless::Term nameless_term = nameless::Abst(nameless::Term(
      nameless::Abst(nameless::Term(nameless::Abst(nameless::Var(-2))))));
  NameContext free_nctx;
  const named::Term named_term = AddNames(nameless_term, &free_nctx);

  const named::Abst* abst = named_term.Get<named::Abst>();
  ASSERT_THAT(abst, NotNull());
  EXPECT_EQ(abst->VarName(), "a");

  const named::Abst* body_abst = abst->Body().Get<named::Abst>();
  ASSERT_THAT(body_abst, NotNull());
  EXPECT_EQ(body_abst->VarName(), "b");

  const named::Abst* body_body_abst = body_abst->Body().Get<named::Abst>();
  ASSERT_THAT(body_body_abst, NotNull());
  EXPECT_EQ(body_body_abst->VarName(), "c");

  const named::Var* var = body_body_abst->Body().Get<named::Var>();
  ASSERT_THAT(var, NotNull());
  EXPECT_EQ(var->Name(), "b");
}

TEST(AddNames, PredefinedNames) {
  NameContext free_nctx;
  free_nctx.SetName("g", 0);

  const nameless::Term nameless_term = nameless::Abst(nameless::Var(0));
  const named::Term named_term = AddNames(nameless_term, &free_nctx);

  const named::Abst* abst = named_term.Get<named::Abst>();
  ASSERT_THAT(abst, NotNull());
  EXPECT_EQ(abst->VarName(), "a");

  const named::Var* body_var = abst->Body().Get<named::Var>();
  ASSERT_THAT(body_var, NotNull());
  EXPECT_EQ(body_var->Name(), "g");
}
}  // namespace
}  // namespace transform
}  // namespace lhat
