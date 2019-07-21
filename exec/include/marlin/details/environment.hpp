#ifndef marlin_exec_environment_hpp
#define marlin_exec_environment_hpp

#include <jsc/jsc.hpp>

namespace marlin::exec {

struct environment {
  inline environment() {
    register_print_callback([](auto string) {});
  }

  template <typename callback_type>
  inline void register_print_callback(callback_type callback) {
    _ctx.root()["print"] = _ctx.callable(
        [&callback](auto ctx, auto this_object, auto args, auto exception) {
          if (args.size() == 0) {
            *exception = ctx.error("To few arguments!");
          } else if (args.size() == 1) {
            std::string str = args[0].to_string();
            if (ctx.ok()) {
              callback(str);
            }
          } else {
            *exception = ctx.error("To many arguments!");
          }
          return ctx.undefined();
        });
  }

  // temporary interface
  inline void execute(std::string javascript,
                      std::string source_url = "<anonymous>") {
    _ctx.clear_exception();
    _ctx.eval_script(javascript, source_url);
  }

 private:
  jsc::context _ctx;
};

}  // namespace marlin::exec

#endif  // marlin_exec_environment_hpp