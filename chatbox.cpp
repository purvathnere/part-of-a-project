#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <cppurses/widget/layouts/horizontal_layout.hpp>
#include <cppurses/system/focus.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

using namespace cppurses;
using namespace std;

vector<string> read_csv(const string& filename) {
    vector<string> names;
    ifstream file(filename);
    string line, name;
    bool first_line = true;
    while (getline(file, line)) {
        if (first_line) {
            first_line = false;
            continue;
        }
        istringstream iss(line);
        getline(iss, name, ',');
        names.push_back(name);
    }
    file.close();
    return names;
}

void group_add(const string& name) {
    vector<string> names = read_csv("Samples/people.csv");
    WebDriver driver;
    driver.get("https://web.whatsapp.com/");
    this_thread::sleep_for(20s);
    try {
        driver.find_element(By::XPATH, "//*[@title='" + name + "']").click();
    } catch (...) {
        cout << "WhatsApp group doesn't exist" << endl;
        cout << "Please try again" << endl;
        return;
    }
    driver.wait_until(EC.visibility_of_element_located(By::XPATH, "//*[@class='_3V5x5']"));
    driver.find_element(By::XPATH, "//*[@class='_3V5x5']").click();
    driver.wait_until(EC.visibility_of_element_located(By::XPATH, "//*[@class='_3p0T6']"));
    driver.find_element(By::XPATH, "//*[@class='_3p0T6']").click();
    for (const auto& i : names) {
        driver.find_element(By::XPATH, "//*[@title='Search…']").send_keys(i);
        this_thread::sleep_for(5s);
        driver.find_element(By::XPATH, "//*[@title='" + i + "']").click();
    }
    driver.find_element(By::XPATH, "//*[@data-icon='checkmark-light']").click();
    this_thread::sleep_for(5s);
    driver.wait_until(EC.visibility_of_element_located(By::XPATH, "//*[@class='_2eK7W _3PQ7V']"));
    driver.find_element(By::XPATH, "//*[@class='_2eK7W _3PQ7V']").click();
}

int main() {
    System sys;
    Horizontal_layout layout;
    Label label{"Enter the name of the WhatsApp group you want to add people to: "};
    layout.add(label);
    line_edit::Line_edit input;
    layout.add(input);
    layout.add(Widget::Height_policy::Fixed_height(1));
    layout.make_page();
    Push_button button{"Add People"};
    button.clicked.connect([&input] {
        group_add(input.contents().str());
        input.clear();
    });
    layout.add(button);
    sys.set_head(layout);
    return sys.run();
}
