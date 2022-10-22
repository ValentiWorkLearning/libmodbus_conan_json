#include "application_ui.hpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

namespace ApplicationUi
{
void launchUiTest()
{
    using namespace ftxui;

    // Define the document
    Element document = hbox({
        text("left") | border,
        text("middle") | border | flex,
        text("right") | border,
    });

    auto screen = Screen::Create(
        Dimension::Full(),       // Width
        Dimension::Fit(document) // Height
    );
    Render(screen, document);
    screen.Print();
}
} // namespace ApplicationUi