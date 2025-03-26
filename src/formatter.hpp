#include "nlohmann_json.hpp"
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "lexbor/css/css.h"
#include "lexbor/html/html.h"
#include "lexbor/selectors/selectors.h"

class Formatter {
  public:
    enum class OutputType { TEXT, ATTRIBUTE, INNER_HTML, OUTER_HTML, JSON };

    Formatter(OutputType output_type,
              const std::optional<std::string> &attr = {},
              const std::string &base_url = "")
        : output_type_(output_type), attr_(attr), base_url_(base_url) {}

    void setLimit(int limit) { limit_ = limit; }
    void setJson(bool is_json) { is_json_ = is_json; }

    void processNode(lxb_dom_node_t *node) {
        if (limit_ > 0 && count_ >= limit_) {
            return;
        }

        std::string result;
        switch (output_type_) {
        case OutputType::TEXT:
            result = getTextContent(node);
            break;
        case OutputType::ATTRIBUTE:
            result = getAttributeValue(node);
            break;
        case OutputType::INNER_HTML:
            result = getInnerHTML(node);
            break;
        case OutputType::OUTER_HTML:
            result = getOuterHTML(node);
            break;
        case OutputType::JSON:
            result = getJson(node);
            break;
        }

        if (is_json_) {
            json_output_.push_back(result);
        } else {
            std::cout << result << std::endl;
        }

        count_++;
    }

    void printJson() {
        if (is_json_) {
            nlohmann::json j = json_output_;
            std::cout << j.dump(2) << std::endl; // Pretty print
        }
    }

  private:
    OutputType output_type_;
    std::optional<std::string> attr_;
    std::string base_url_;
    int count_ = 0;
    int limit_ = 0;
    bool is_json_ = false;
    std::vector<std::string> json_output_;

    std::string getTextContent(lxb_dom_node_t *node) {
        size_t len;
        lxb_char_t *data = lxb_dom_node_text_content(node, &len);
        return std::string(reinterpret_cast<const char *>(data), len);
    }

    std::string getAttributeValue(lxb_dom_node_t *node) {
        if (!attr_) {
            return "";
        }
        bool apply_base_path_check = false;

        std::string node_name = std::string(
            reinterpret_cast<const char *>(lxb_dom_node_name(node, nullptr)));
        if (attr_ == "href" && node_name == "A") {
            // Apply base path checks
            apply_base_path_check = true;
        }

        const lxb_dom_attr_t *val = lxb_dom_element_attr_by_name(
            lxb_dom_interface_element(node),
            reinterpret_cast<const lxb_char_t *>(attr_->c_str()),
            attr_->size());

        if (val && val->value->length) {
            auto value =
                std::string(reinterpret_cast<const char *>(val->value->data),
                            val->value->length);
            return !apply_base_path_check ? value
                                          : normalise_url(value, base_url_);
        }

        return "";
    }

    std::string getInnerHTML(lxb_dom_node_t *node) {
        // Serialize inner HTML content (children of the node)
        std::ostringstream oss;
        (void)lxb_html_serialize_deep_cb(
            node,
            [](const lxb_char_t *data, size_t len, void *ctx) -> lxb_status_t {
                std::ostringstream *oss =
                    reinterpret_cast<std::ostringstream *>(ctx);
                *oss << std::string(reinterpret_cast<const char *>(data), len);
                return LXB_STATUS_OK;
            },
            &oss);

        return oss.str();
    }

    std::string getOuterHTML(lxb_dom_node_t *node) {
        // Serialize the full HTML content (node + children)
        std::ostringstream oss;
        (void)lxb_html_serialize_tree_cb(
            node,
            [](const lxb_char_t *data, size_t len, void *ctx) -> lxb_status_t {
                std::ostringstream *oss =
                    reinterpret_cast<std::ostringstream *>(ctx);
                *oss << std::string(reinterpret_cast<const char *>(data), len);
                return LXB_STATUS_OK;
            },
            &oss);

        return oss.str();
    }

    std::string getJson(lxb_dom_node_t *node) {
        // For simplicity, just print the outer HTML as JSON
        return getOuterHTML(node);
    }

    std::string normalise_url(std::string url, std::string base_url) {
        return is_absolute_url(url) ? url : base_url + url;
    }

    bool is_absolute_url(const std::string &url) {
        // Starts with known schemes
        return url.starts_with("http://") || url.starts_with("https://") ||
               url.starts_with("ftp://") || url.starts_with("mailto:") ||
               url.starts_with("data:") || url.starts_with("tel:") ||
               url.starts_with("//"); // protocol-relative
    }
};