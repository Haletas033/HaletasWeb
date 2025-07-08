#ifndef TAG_CLASS_H
#define TAG_CLASS_H

#include<string>

class Tag {
protected:
    std::string name, content;
    std::map<std::string, std::string> attrs;
    std::vector<Tag> children;

public:
    Tag(const std::string& name) : name(name) {}

    //Adds text content
    Tag& operator<<(const std::string& text) {
        content += text;
        return *this;
    }

    //Adds another tag inside this tag (nesting)
    Tag& operator<<(const Tag& other) {
        children.push_back(other);
        return *this;
    }

    //Adds an attribute like class, id, style, etc.
    Tag& addAttr(const std::string& key, const std::string& value) {
        attrs[key] = value;
        return *this;
    }

    //Convert to final HTML string
    [[nodiscard]] virtual std::string str() const {
        std::ostringstream oss;
        oss << "<" << name;

        for (const auto &[k, v] : attrs) {
            oss << " " << k << "=\"" << v << "\"";
        }

        oss << ">";
        oss << content;

        for (const auto &child : children) {
            oss << child.str();
        }
        oss << "</" << name << ">";
        return oss.str();
    }

    [[nodiscard]] virtual std::string Head() const {
        return "<!DOCTYPE html>\n"
               "<html lang=\"en\">\n"
               "<head>\n"
               "<meta charset=\"UTF-8\">\n"
               "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
               "<link href=\"https://cdn.jsdelivr.net/npm/@picocss/pico@2/css/pico.min.css\" rel=\"stylesheet\">"
               "<title>HaletasWeb</title>\n"

               "</head>\n"
               "<body>\n";
    }
};

#endif //TAG_CLASS_H