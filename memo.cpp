#include <iostream>

int main() {
    double a, b;
    char op;

    std::cout << "계산식 입력 (예: 3 + 5): ";
    std::cin >> a >> op >> b;

    switch (op) {
        case '+': std::cout << "결과: " << a + b << std::endl; break;
        case '-': std::cout << "결과: " << a - b << std::endl; break;
        case '*': std::cout << "결과: " << a * b << std::endl; break;
        case '/':
            if (b == 0) std::cout << "오류: 0으로 나눌 수 없습니다." << std::endl;
            else std::cout << "결과: " << a / b << std::endl;
            break;
        default:
            std::cout << "오류: 지원하지 않는 연산자입니다." << std::endl;
    }

    return 0;
}
