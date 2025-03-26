#include <string>

class Config {
  public:
    Config() {}

    std::string filename = "-";
    std::string slctrs = "";
    std::string attr = "";
    std::string base_url = "";
    bool first = false;
    bool text = false;
    bool inner = false;
    bool json = false;
    int limit = -1;
};