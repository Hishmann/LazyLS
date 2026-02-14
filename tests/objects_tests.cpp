#include <gtest/gtest.h>

#include <objects.h>
#include <util.h>
#include <global.h>


TEST(EventQueueTest, Behavior) {
    EventQueue e;

    ASSERT_TRUE(e.empty());

    Event t; t.type = EventType::KEY_PRESS; t.keyboard.key = 'a';

    e.push(t);

    EXPECT_FALSE(e.empty());

    std::vector<Event> array = e.pop_all();

    EXPECT_TRUE(e.empty());
    EXPECT_FALSE(array.empty());
    EXPECT_EQ(array.size(),1);
}


TEST(BoxElementTest, Behavior) {

    Event t; t.type = EventType::KEY_PRESS; t.keyboard.key = 'a';

    std::unique_ptr<BaseRenderElement> test_box = std::make_unique<BoxRenderElement>(
        2, 2, PixelCoordinates{1,1}, style_rgb_code({}, std::nullopt, RGB_FB{204,0,0}), 
        [](BoxRenderElement& b, const Event& e) {
        if (e.type == EventType::KEY_PRESS && e.keyboard.key == 'a') {
            b.coord.x += 1;
            return true;
        }
        return false;
    });

    std::string test_output_1;
    test_output_1 += "\e[1;1H" + style_rgb_code({}, std::nullopt, RGB_FB{204,0,0}) + "  " +"\e[0m";
    test_output_1 += "\e[2;1H" + style_rgb_code({}, std::nullopt, RGB_FB{204,0,0}) + "  " +"\e[0m";

    EXPECT_EQ(test_box -> representation(), test_output_1);

    test_box -> update(t);

    std::string test_output_2;
    test_output_2 += "\e[1;2H" + style_rgb_code({}, std::nullopt, RGB_FB{204,0,0}) + "  " +"\e[0m";
    test_output_2 += "\e[2;2H" + style_rgb_code({}, std::nullopt, RGB_FB{204,0,0}) + "  " +"\e[0m";

    EXPECT_EQ(test_box -> representation(), test_output_2);

}