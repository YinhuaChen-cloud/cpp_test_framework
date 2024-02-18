#include <iostream>
#include <vector>
#include <string>

// 测试用例类
class TestCase {
public:
    TestCase(const std::string& name) : testName(name) {}
    virtual ~TestCase() {}

    // 纯虚函数，子类需要实现此函数以执行具体的测试逻辑
    virtual void run() = 0;

    std::string getName() const {
        return testName;
    }

protected:
    std::string testName;
};

// 测试用例管理器
class TestManager {
public:
    void addTest(TestCase* test) {
        tests.push_back(test);
    }

    void runTests() {
        for (auto& test : tests) {
            try {
                test->run();
                std::cout << "Test Passed: " << test->getName() << std::endl;
                passedTests.push_back(test->getName());
            } catch (const std::exception& e) {
                std::cout << "Test Failed: " << test->getName() << " - Reason: " << e.what() << std::endl;
                failedTests.push_back(test->getName());
            }
        }
    }

    void reportResults() const {
        std::cout << "\nTest Results:\n";

	std::cout << "\033[32m";
        std::cout << "Passed Tests:\n";
        for (const auto& testName : passedTests) {
            std::cout << testName << std::endl;
        }
	std::cout << "\033[0m";

	std::cout << "\033[31m";
        std::cout << "\nFailed Tests:\n";
        for (const auto& testName : failedTests) {
            std::cout << testName << std::endl;
        }
	std::cout << "\033[0m";
    }

private:
    std::vector<TestCase*> tests;  // 所有的测试用例
    std::vector<std::string> passedTests;  // 通过的测试用例
    std::vector<std::string> failedTests;  // 失败的测试用例
};

// 具体的测试用例 -------------------------------------------------------- start
class MyTest1 : public TestCase {
public:
    MyTest1() : TestCase("1. Load_PUT_base64_CFG") {}

    void run() override {
        // 这里放置测试逻辑
        // 如果测试失败，可以抛出异常
    }
};

class MyTest2 : public TestCase {
public:
    MyTest2() : TestCase("2. OuterLoop_and_InnerLoop") {}

    void run() override {
        // 测试逻辑
        throw std::runtime_error("Example failure");
    }
};
// 具体的测试用例 -------------------------------------------------------- end

// 开始执行所有测试用例
int main() {
    TestManager manager;
    manager.addTest(new MyTest1());
    manager.addTest(new MyTest2());

    manager.runTests();
    manager.reportResults();

    return 0;
}



