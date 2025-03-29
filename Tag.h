#ifndef TAG_CLASS_H
#define TAG_CLASS_H

#include<string>

class Tag {
    std::string name, content, attributes;

public:
    Tag(const std::string& name) : name(name) {}

    //Adds text content
    Tag& operator<<(const std::string& text) {
        content += text;
        return *this;
    }

    //Adds another tag inside this tag (nesting)
    Tag& operator<<(const Tag& other) {
        content += other.str();
        return *this;
    }

    //Adds an attribute like class, id, style, etc.
    Tag& addAttr(const std::string& key, const std::string& value) {
        attributes += " " + key + "=\"" + value + "\"";
        return *this;
    }

    //Convert to final HTML string
    std::string str() const {
        return "<" + name + attributes + ">" + content + "</" + name + ">";
    }
};

#endif //!TAG_CLASS_H1