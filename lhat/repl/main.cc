#include <fstream>
#include <iostream>
#include <string>

#include "lhat/named/alpha.h"
#include "lhat/named/ast.h"
#include "lhat/named/parse.h"
#include "lhat/named/printer.h"
#include "lhat/nameless/ast.h"
#include "lhat/nameless/beta.h"
#include "lhat/repl/const_env.h"
#include "lhat/repl/parse.h"
#include "lhat/transform/names.h"
#include "lhat/transform/types.h"
#include "lhat/util/line_transform_buf.h"

namespace lhat {
namespace repl {
namespace {
std::string BoolToString(bool b) {
  if (b) {
    return std::string("true");
  } else {
    return std::string("false");
  }
}

std::string ToNamedTermString(const nameless::Term& term,
                              transform::NameContext* free_nctx) {
  const named::Term output_term = transform::AddNames(term, free_nctx);
  std::string output;
  named::Printer::Print(output_term, &output);
  return output;
}

util::ErrorOr<std::string> Def(ConstEnv* consts, std::istream* input_stream) {
  const util::ErrorOr<std::string> const_name = ParseConstName(input_stream);
  RETURN_IF_ERROR(const_name);

  const util::ErrorOr<named::Term> term = named::Parse(input_stream);
  RETURN_IF_ERROR(term);

  std::string term_str;
  named::Printer::Print(term.Value(), &term_str);
  consts->Set(const_name.Value(), term_str);

  return std::string("");
}

util::ErrorOr<std::string> IsAlphaEquiv(std::istream* input_stream) {
  const util::ErrorOr<named::Term> first_term = named::Parse(input_stream);
  RETURN_IF_ERROR(first_term);

  const util::ErrorOr<named::Term> second_term = named::Parse(input_stream);
  RETURN_IF_ERROR(second_term);

  return BoolToString(
      named::IsAlphaEquiv(first_term.Value(), second_term.Value()));
}

util::ErrorOr<std::string> IsBetaRedex(std::istream* input_stream) {
  const util::ErrorOr<named::Term> input_term = named::Parse(input_stream);
  RETURN_IF_ERROR(input_term);

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term.Value(), &free_nctx);

  return BoolToString(nameless::IsBetaRedex(term));
}

util::ErrorOr<std::string> IsBetaNormal(std::istream* input_stream) {
  const util::ErrorOr<named::Term> input_term = named::Parse(input_stream);
  RETURN_IF_ERROR(input_term);

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term.Value(), &free_nctx);

  return BoolToString(nameless::IsBetaNormalForm(term));
}

util::ErrorOr<std::string> IsHeadNormal(std::istream* input_stream) {
  const util::ErrorOr<named::Term> input_term = named::Parse(input_stream);
  RETURN_IF_ERROR(input_term);

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term.Value(), &free_nctx);

  return BoolToString(nameless::IsHeadNormalForm(term));
}

util::ErrorOr<std::string> BetaReduce(std::istream* input_stream) {
  const util::ErrorOr<named::Term> input_term = named::Parse(input_stream);
  RETURN_IF_ERROR(input_term);

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term.Value(), &free_nctx);

  nameless::BetaReduceTerm(&term);

  return ToNamedTermString(term, &free_nctx);
}

util::ErrorOr<std::string> EvalAppl(std::istream* input_stream) {
  const util::ErrorOr<named::Term> input_term = named::Parse(input_stream);
  RETURN_IF_ERROR(input_term);

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term.Value(), &free_nctx);

  while (nameless::BetaReduceAppl(&term)) {
    // Normalize the term.
  }

  return ToNamedTermString(term, &free_nctx);
}

util::ErrorOr<std::string> EvalNormal(std::istream* input_stream) {
  const util::ErrorOr<named::Term> input_term = named::Parse(input_stream);
  RETURN_IF_ERROR(input_term);

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term.Value(), &free_nctx);

  while (nameless::BetaReduceNormal(&term)) {
    // Normalize the term.
  }

  return ToNamedTermString(term, &free_nctx);
}

util::ErrorOr<std::string> InferType(std::istream* input_stream) {
  const util::ErrorOr<named::Term> input_term = named::Parse(input_stream);
  RETURN_IF_ERROR(input_term);

  transform::NameContext free_nctx;
  nameless::Term term = transform::RemoveNames(input_term.Value(), &free_nctx);

  const std::unordered_map<int, int> free_var_types;
  std::vector<transform::Type> types;
  std::vector<transform::TypeConstraint> constraints;
  const util::ErrorOr<int> term_type_idx =
      transform::CollectTypes(term, free_var_types, &types, &constraints);
  RETURN_IF_ERROR(term_type_idx);

  std::unordered_map<transform::SimpleType, int> subs;
  if (!transform::UnifyTypes(types, constraints, &subs)) {
    return util::Error("Failed to infer type");
  }

  transform::ApplyTypeSubs(subs, &types);

  return transform::TypeToString(types, term_type_idx.Value());
}

util::ErrorOr<std::string> ExecuteCommand(const std::string& command,
                                          ConstEnv* consts,
                                          std::istream* input_stream) {
  if (command.empty()) {
    return std::string("");
  } else if (command == "def") {
    return Def(consts, input_stream);
  } else if (command == "alpha-equiv?") {
    return IsAlphaEquiv(input_stream);
  } else if (command == "beta-redex?") {
    return IsBetaRedex(input_stream);
  } else if (command == "beta-normal?") {
    return IsBetaNormal(input_stream);
  } else if (command == "head-normal?") {
    return IsHeadNormal(input_stream);
  } else if (command == "beta-reduce") {
    return BetaReduce(input_stream);
  } else if (command == "eval-appl") {
    return EvalAppl(input_stream);
  } else if (command == "eval-normal") {
    return EvalNormal(input_stream);
  } else if (command == "infer-type") {
    return InferType(input_stream);
  } else {
    return std::string("Unknown command: ") + command;
  }
}

void PrintCommandOutput(const util::ErrorOr<std::string>& output) {
  if (!output.Ok()) {
    std::cout << output.Error().Message() << std::endl;
    return;
  }

  if (!output.Value().empty()) {
    std::cout << output.Value() << std::endl;
  }
}

void LoadConstsFromFile(const std::string& file_name, ConstEnv* consts) {
  std::ifstream file(file_name, std::ios::in);
  if (!file.is_open()) {
    std::cout << "Failed to open file: " << file_name << std::endl;
    return;
  }

  while (!file.eof()) {
    util::LineTransformBuf const_resolve_buf(
        &file, [consts](std::string* line) -> bool {
          const std::optional<util::Error> resolve_error =
              consts->Resolve(line);
          if (resolve_error) {
            std::cout << "Failed to resolve const: " << resolve_error->Message()
                      << std::endl;
            return false;
          }
          return true;
        });
    std::istream input_stream(&const_resolve_buf);

    const util::ErrorOr<std::string> command = ParseCommand(&input_stream);
    if (!command.Ok()) {
      std::cout << "Failed to parse command: " << command.Error().Message()
                << std::endl;
      continue;
    }

    ExecuteCommand(command.Value(), consts, &input_stream);
  }
}

void Run(int argc, char* argv[]) {
  ConstEnv consts;

  for (int i = 1; i < argc; i += 2) {
    if (std::strcmp(argv[i], "--exec") == 0) {
      if (i + 1 >= argc) {
        std::cout << "Expected path to file" << std::endl;
      } else {
        LoadConstsFromFile(argv[i + 1], &consts);
      }
    } else {
      std::cout << "Unknown flag: " << argv[i] << std::endl;
    }
  }

  while (true) {
    std::cout << "> ";

    util::LineTransformBuf const_resolve_buf(
        &std::cin, [&consts](std::string* line) -> bool {
          const std::optional<util::Error> resolve_error = consts.Resolve(line);
          if (resolve_error) {
            std::cout << "Failed to resolve const: " << resolve_error->Message()
                      << std::endl;
            return false;
          }
          return true;
        });
    std::istream input_stream(&const_resolve_buf);

    const util::ErrorOr<std::string> command = ParseCommand(&input_stream);
    if (!command.Ok()) {
      std::cout << "Failed to parse command: " << command.Error().Message()
                << std::endl;
      continue;
    }

    if (command.Value() == "exit") {
      break;
    }

    const util::ErrorOr<std::string> output =
        ExecuteCommand(command.Value(), &consts, &input_stream);
    PrintCommandOutput(output);
  }
}
}  // namespace
}  // namespace repl
}  // namespace lhat

int main(int argc, char* argv[]) { lhat::repl::Run(argc, argv); }
