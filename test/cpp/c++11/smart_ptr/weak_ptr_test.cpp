//test copy from cppreference.
//

#include <iostream>
#include "shared_ptr.h"
using namespace obtuse;

weak_ptr<int> gwp;
 
void observe_gwp() {
    std::cout << "use_count(): " << gwp.use_count() << "\t id: ";
    if (auto sp = gwp.lock())
        std::cout << *sp << '\n';
    else
        std::cout << "??\n";
}
 
void share_recursively(shared_ptr<int> sp, int depth) {
    observe_gwp(); // : 2 3 4
    if (1 < depth)
        share_recursively(sp, depth - 1);
    observe_gwp(); // : 4 3 2
}
 
int main() {
    observe_gwp(); // : 0
    {
        auto sp = make_shared<int>(42);
        gwp = sp;
        observe_gwp(); // : 1
        share_recursively(sp, 3); // : 2 3 4 4 3 2
        observe_gwp(); // : 1
    }
    observe_gwp(); // : 0
}