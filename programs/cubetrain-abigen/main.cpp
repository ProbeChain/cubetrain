#include <fc/exception/exception.hpp>
#include <fc/io/json.hpp>
#include <cubetrain/abi_generator/abi_generator.hpp>
#include <fc/variant_object.hpp>

using namespace cubetrain;
using namespace cubetrain::chain;

using mvo = fc::mutable_variant_object;

std::unique_ptr<FrontendActionFactory> create_factory(bool verbose, bool opt_sfs, string abi_context, abi_def& output, const string& contract, const vector<string>& actions) {

  struct abi_frontend_action_factory : public FrontendActionFactory {

    bool                   verbose;
    bool                   opt_sfs;
    string                 abi_context;
    abi_def&               output;
    const string&          contract;
    const vector<string>&  actions;

    abi_frontend_action_factory(bool verbose, bool opt_sfs, string abi_context,
      abi_def& output, const string& contract, const vector<string>& actions) : verbose(verbose),
      abi_context(abi_context), output(output), contract(contract), actions(actions) {}

    clang::FrontendAction *create() override {
      return new generate_abi_action(verbose, opt_sfs, abi_context, output, contract, actions);
    }

  };

  return std::unique_ptr<FrontendActionFactory>(
      new abi_frontend_action_factory(verbose, opt_sfs, abi_context, output, contract, actions)
  );
}

std::unique_ptr<FrontendActionFactory> create_find_macro_factory(string& contract, vector<string>& actions, string abi_context) {

  struct abi_frontend_macro_action_factory : public FrontendActionFactory {

    string&          contract;
    vector<string>&  actions;
    string           abi_context;

    abi_frontend_macro_action_factory (string& contract, vector<string>& actions,
      string abi_context ) : contract(contract), actions(actions), abi_context(abi_context) {}

    clang::FrontendAction *create() override {
      return new find_cubetrain_abi_macro_action(contract, actions, abi_context);
    }

  };

  return std::unique_ptr<FrontendActionFactory>(
    new abi_frontend_macro_action_factory(contract, actions, abi_context)
  );
}

static cl::OptionCategory abi_generator_category("ABI generator options");

static cl::opt<std::string> abi_context(
    "context",
    cl::desc("ABI context"),
    cl::cat(abi_generator_category));

static cl::opt<std::string> abi_destination(
    "destination-file",
    cl::desc("destination json file"),
    cl::cat(abi_generator_category));

static cl::opt<bool> abi_verbose(
    "verbose",
    cl::desc("show debug info"),
    cl::cat(abi_generator_category));

static cl::opt<bool> abi_opt_sfs(
    "optimize-sfs",
    cl::desc("Optimize single field struct"),
    cl::cat(abi_generator_category));

int main(int argc, const char **argv) { abi_def output; try {
   CommonOptionsParser op(argc, argv, abi_generator_category);
   ClangTool Tool(op.getCompilations(), op.getSourcePathList());

   string contract;
   vector<string> actions;
   int result = Tool.run(create_find_macro_factory(contract, actions, abi_context).get());
   if(!result) {
      result = Tool.run(create_factory(abi_verbose, abi_opt_sfs, abi_context, output, contract, actions).get());
      if(!result) {
         abi_serializer abis(output, fc::seconds(1)); // No risk to client side serialization taking a long time
         fc::variant vabi;
         to_variant(output, vabi);

         auto comment = fc::format_string(
           "This file was generated by cubetrain-abigen. DO NOT EDIT - ${ts}",
           mvo("ts",fc::time_point_sec(fc::time_point::now()).to_iso_string()));

         auto abi_with_comment = mvo("____comment", comment)(mvo(vabi));

         fc::json::save_to_file(abi_with_comment, abi_destination, true);
      }
   }
   return result;
} FC_CAPTURE_AND_LOG((output)); return -1; }
