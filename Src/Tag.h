#ifndef TAG_CLASS_H
#define TAG_CLASS_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

class Tag {
protected:
    std::string name, content;
    std::map<std::string, std::string> attrs;
    std::vector<Tag> children;

    bool forceSelfCLosing = false;
    bool forceNotSelfClosing = false;

    static const std::unordered_set<std::string> &voidTags() {
        static const std::unordered_set<std::string> tags = {
            "area", "base", "br", "col", "embed", "hr", "img",
            "input", "link", "meta", "source", "track", "wbr"
        };
        return tags;
    }

public:
    Tag(const std::string& name) : name(name) {}

    //Adds text content
    Tag& operator<<(const std::string& text) {
        content += text;
        return *this;
    }

    //Adds another tag inside this tag (nesting)
    Tag& put(const Tag& child) {
        children.push_back(child);
        return *this;
    }

    Tag& text(const std::string& txt) {
        content += txt;
        return *this;
    }

    //Adds an attribute like class, id, style, etc.
    Tag& addAttr(const std::string& key, const std::string& value) {
        attrs[key] = value;
        return *this;
    }

    Tag& selfClosing(){forceSelfCLosing = true; return *this;}
    Tag& selfNotClosing(){forceNotSelfClosing = true; return *this;}

    //Convert to final HTML string
    [[nodiscard]] virtual std::string str() const {
        std::ostringstream oss;
        oss << "<" << name;

        for (const auto &[k, v] : attrs) {
            oss << " " << k << "=\"" << v << "\"";
        }

        if (bool selfClosing = forceSelfCLosing || (voidTags().count(name) && !forceNotSelfClosing)) {
            oss << "/>";
        } else {
            oss << ">";
            oss << content;
            for (const auto &child : children) {
                oss << child.str();
            }
            oss << "</" << name << ">";
        }

        return oss.str();
    }
};

inline void WriteHTML(const std::string &filename, const Tag &header, const Tag &main, const std::string &title, const Tag &extraHead = Tag("")) {
    try {
        std::filesystem::create_directories("out");

        //Copy docs imgs
        std::filesystem::copy("LSIMdocs/imgs", "out/imgs",
        std::filesystem::copy_options::recursive |
        std::filesystem::copy_options::overwrite_existing);

        //Copy skills imgs
        std::filesystem::copy("skills", "out/imgs/skills",
        std::filesystem::copy_options::recursive |
        std::filesystem::copy_options::overwrite_existing);

    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Warning: could not copy imgs: " << e.what() << '\n';
    }


    std::ofstream htmlFile("out/" + filename);


    if (!htmlFile.is_open()) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    Tag html = Tag("html").addAttr("lang", "en");

    Tag head("head");
    head
        .put(Tag("meta").addAttr("charset", "UTF-8"))
        .put(Tag("meta").addAttr("name", "viewport").addAttr("content", "width=device-width, initial-scale=1.0"))
        .put(Tag("link").addAttr("href", "https://cdn.jsdelivr.net/npm/@picocss/pico@2/css/pico.min.css").addAttr("rel", "stylesheet"))
        /*test stylesheet*/
        .put(Tag("link").addAttr("rel", "stylesheet").addAttr("href", "styles/style.css"))

        //Prism for syntax highlighting
        .put(Tag("link").addAttr("href", "https://cdn.jsdelivr.net/npm/prismjs@1.30.0/themes/prism-tomorrow.css").addAttr("rel", "stylesheet"))
        .put(Tag("script").addAttr("src", "https://cdn.jsdelivr.net/npm/prismjs@1.30.0/prism.min.js"))

        //json
        .put(Tag("script").addAttr("src", "https://cdn.jsdelivr.net/npm/prismjs@1.30.0/components/prism-json.min.js"))
        //C-like
        .put(Tag("script").addAttr("src", "https://unpkg.com/prismjs@1.30.0/components/prism-c.min.js"))

        //Mathjax for latex support
        .put(Tag("script")
            .addAttr("src", "https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-chtml.js"))

        //Matter.js for a bit of physics
        .put(Tag("script")
                .addAttr("src", "https://cdnjs.cloudflare.com/ajax/libs/matter-js/0.18.0/matter.min.js"))

        .put(extraHead)
        .put(Tag("title").text(title));

    Tag body("body");

    body.put(header).put(main);

    html.put(head).put(body);

    htmlFile << "<!DOCTYPE html>\n" << html.str();
}

inline Tag div() { return Tag("div"); }
inline Tag h1(const std::string& text = "") { return Tag("h1") << text; }
inline Tag h2(const std::string& text = "") { return Tag("h2") << text; }
inline Tag h3(const std::string& text = "") { return Tag("h3") << text; }
inline Tag p(const std::string& text = "") { return Tag("p") << text; }

class DocumentBuilder {
    Tag html{"html"};

public:
    DocumentBuilder& put(const Tag& child) {
        html.put(child);
        return *this;
    }
};

#endif //TAG_CLASS_H