//
// Created by halet on 2/21/2026.
//

#ifndef STYLE_H
#define STYLE_H
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>

//@font-face can be written as a Style
#define FontFace(var) Style var("@font-face")

//Different at rules
#define Media(var, args) AtRule var("@media " + std::string(args))
#define Supports(var, args) AtRule var("@supports " + std::string(args))
#define Keyframes(var, easingFunc) AtRule var("@keyframes " + std::string(easingFunc))

class AtRule;
class Style;
class Import;

struct StyleData {
    std::vector<Import*> imports;
    std::vector<AtRule*> atRules;
    std::vector<Style*> styles;
};

inline static thread_local StyleData styleData;

class Import {
private:
    std::tuple<std::string, std::string> import;
public:
    //Suppresses lifetime warning
    static void Add(const std::string& type, const std::string& name) {
        new Import(type, name);
    }

    Import(const std::string & type, const std::string & name) : import(type, name) {
        styleData.imports.push_back(this);
    }

    friend class Builder;
};

class Style {
    std::string element;
    std::vector<std::tuple<std::string, std::string>> properties;


public:
    explicit Style(std::string s) : element(std::move(s)) {
        styleData.styles.push_back(this);
    }

    Style& put(const std::string& name, const std::string& value) {
        properties.emplace_back(name, value);
        return *this;
    }

    friend class Builder;
};

class AtRule {
private:
    const std::string name;
    std::vector<Style*> styles;
public:
    explicit AtRule(std::string n) : name(std::move(n)) {
        styleData.atRules.push_back(this);
    }

    AtRule& put(Style& style) {
        auto& s = styleData.styles;
        //Remove the styles from the old styles vector
        s.erase(std::remove(s.begin(), s.end(), &style), s.end());
        this->styles.push_back(&style);
        return *this;
    }

    friend class Builder;
};

class Builder {
private:
    static void BuildImports(std::string& output) {
        for (const Import *import : styleData.imports)
            output += "@import " + std::get<0>(import->import) + "(\"" + std::get<1>(import->import) + "\");\n";
    }

    static void BuildStyles(std::string& output, const std::vector<Style*> &styles) {
        for (const Style *style : styles) {
            //Start style
            output += style->element + "{\n";
            //Add properties
            for (std::tuple property : style->properties) {
                output += std::get<0>(property) + ':' + std::get<1>(property) + ";\n";
            }
            //Close style
            output += "}\n";
        }
    }

    static void BuildAtRules(std::string& output) {
        for (const AtRule *atRule : styleData.atRules) {
            //Start style
            output += atRule->name + "{\n";
            //Add properties
            BuildStyles(output, atRule->styles);
            //Close style
            output += "}\n";
        }
    }
public:
    static void Build(const std::string& fileName) {
        std::string output;

        BuildImports(output);
        BuildAtRules(output);
        BuildStyles(output, styleData.styles);

        std::ofstream cssFile("out/styles/" + fileName + ".css");
        cssFile << output;
    }
};

#endif //STYLE_H
