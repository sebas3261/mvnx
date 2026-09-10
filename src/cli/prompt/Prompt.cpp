#include "Prompt.h"
#include "Style.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static void renderOptions(
    const string &label,
    const vector<string> &options,
    size_t selected
)
{
    cout
        << Style::CYAN
        << "◇  "
        << Style::RESET
        << Style::BOLD
        << label
        << Style::RESET
        << '\n';

    for (size_t i = 0; i < options.size(); i++)
    {
        cout
            << Style::DIM
            << "│  "
            << Style::RESET;

        if (i == selected)
        {
            cout
                << Style::CYAN
                << "› "
                << Style::RESET
                << Style::BOLD
                << options[i]
                << Style::RESET;
        }
        else
        {
            cout << "  " << options[i];
        }

        cout << '\n';
    }

}

string Prompt::text(
    const string &label,
    const string &defaultValue,
    bool preload
)
{
    string prompt;

    prompt += Style::CYAN;
    prompt += "◇  ";
    prompt += Style::RESET;

    prompt += Style::BOLD;
    prompt += label;
    prompt += Style::RESET;

    if (!preload && !defaultValue.empty())
    {
        prompt += " ";
        prompt += Style::DIM;
        prompt += "(" + defaultValue + ")";
        prompt += Style::RESET;
    }

    prompt += "\n";

    prompt += Style::DIM;
    prompt += "│  ";
    prompt += Style::RESET;

    prompt += Style::CYAN;
    prompt += "› ";
    prompt += Style::RESET;

    if (preload && !defaultValue.empty())
    {
        rx.set_preload_buffer(defaultValue);
    }

    const char *input = rx.input(prompt.c_str());

    if (input == nullptr)
    {
        return "";
    }

    string value = input;

    if (value.empty())
    {
        value = defaultValue;
    }

    cout
        << Style::DIM
        << "│"
        << Style::RESET
        << "\n";

    return value;
}

string Prompt::select(
    const string &label,
    const vector<string> &options
)
{
    if (options.empty())
    {
        return "";
    }

    size_t selected = 0;

    auto repaint = [&]()
    {
        cout
            << "\033["
            << options.size() + 1
            << "A"
            << "\033[J";

        renderOptions(label, options, selected);
        cout.flush();
    };

    renderOptions(label, options, selected);

    rx.bind_key(
        replxx::Replxx::KEY::UP,
        [&](char32_t)
        {
            selected = selected == 0 ? options.size() - 1 : selected - 1;
            rx.invoke(replxx::Replxx::ACTION::CLEAR_SELF, 0);
            repaint();
            return replxx::Replxx::ACTION_RESULT::CONTINUE;
        }
    );

    rx.bind_key(
        replxx::Replxx::KEY::DOWN,
        [&](char32_t)
        {
            selected = (selected + 1) % options.size();
            rx.invoke(replxx::Replxx::ACTION::CLEAR_SELF, 0);
            repaint();
            return replxx::Replxx::ACTION_RESULT::CONTINUE;
        }
    );

    string prompt;

    prompt += Style::DIM;
    prompt += "│  ";
    prompt += Style::RESET;

    prompt += Style::CYAN;
    prompt += "Press ↑/↓ and Enter";
    prompt += Style::RESET;
    prompt += " ";

    const char *input = rx.input(prompt.c_str());

    if (input == nullptr)
    {
        return "";
    }

    return options[selected];
}
