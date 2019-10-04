#include "collection.h"

Collection::Collection() {
    std::vector<int> m_indexes = {};
    std::string m_head = "";
    std::string m_tail = "";
}

Collection::Collection(const std::string &head, const std::string &tail, const std::vector<int> &indexes) 
    :m_head(head), m_tail(tail), m_indexes(indexes)
{
    this->_findHoles();
    this->_separate();
}

int Collection::count() {
    return this->m_indexes.size();
}

int Collection::first() {
    return this->m_indexes.front();
}

int Collection::last() {
    return this->m_indexes.back();
}

void Collection::setHead(std::string head){
    m_head = head;
}

std::string Collection::getHead() const{
    return m_head;
}

void Collection::info() {
    std::cout << "Collection construct" << std::endl;
    std::cout << "  head...: " << m_head << std::endl;
    std::cout << "  tail...: " << m_tail << std::endl;
    std::cout << "  indexes: " ;
    for (std::vector<int>::const_iterator i = m_indexes.begin(); i != m_indexes.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    std::cout << "  holes: ";
    for (std::vector<int>::const_iterator i = m_holes.begin(); i != m_holes.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
}

// std::string Collection::format(const std::string &format) {
//     return format;
// }

std::string Collection::format() {
    std::string fullRange = "";
    std::deque<std::string> rangeTokens;

    for(Range r : m_ranges){
        if(r.isSingleFrame) {
            rangeTokens.push_back(std::to_string(r.start));
        } else {
            auto val = (r.step==1) ? 
                std::to_string(r.start) + ":" + std::to_string(r.end) :
                std::to_string(r.start) + ":" + std::to_string(r.end) + "x" + std::to_string(r.step);
            
            rangeTokens.push_back(val);
        }
    }

    fullRange = boost::algorithm::join(rangeTokens, ",");

    return fmt::format("{head}[{range}]{tail}",  
        "head"_a=m_head, "range"_a=fullRange, "tail"_a=m_tail);
}

std::vector<std::string> Collection::getFileList() const{
    std::vector<std::string> result;
    for(auto i : m_indexes){
        result.push_back(fmt::format("{head}{range}{tail}",  
            "head"_a=m_head, "range"_a=i, "tail"_a=m_tail));
    }
    return result;
}

void Collection::_findHoles() {

    // If length > 0
    m_holes.clear();

    int prev = m_indexes.at(0);
    int current;

    for (std::vector<int>::const_iterator i = m_indexes.begin()+1; i != m_indexes.end(); ++i) {
        current = *i;
        while (current-prev != 1) {
            m_holes.push_back(++prev);
        }
        ++prev;
    }
}

/**

*/
void Collection::_separate(){

    Range r;
    m_ranges.clear();

    if(m_indexes.empty()) {
        return;
    } 

    else if (m_indexes.size() == 1) {
        r.isSingleFrame = true;
        r.start = m_indexes.at(0);
        r.end = r.start;
        r.step = 1;

        m_ranges.push_back(r);
        return;
    }

    else if (m_indexes.size() == 2) {
        // 2 indexes collection are always treated as 2 single frames if not consecutives
        // TODO if not consistent when more than 3 where list is: 1,2,8 for instance > considere it as a range
        r.isSingleFrame = true;
        r.start = m_indexes.at(0);
        r.end = r.start;
        r.step = 1;
        m_ranges.push_back(r);

        r.isSingleFrame = true;
        r.start = m_indexes.at(1);
        r.end = r.start;
        r.step = 1;
        
        m_ranges.push_back(r);
        return;
    }

    // ------------------------------------------------------------------------------------
    // Handle a vector of more than 2 items
    // cout << "Separate more than 2 elems:" << endl;
    std::vector<int>::iterator it;

    // Init first range
    r.isSingleFrame = true;
    r.start = m_indexes.at(0);
    r.end = m_indexes.at(0);
    r.step = m_indexes.at(1) - m_indexes.at(0);

    // TOFIX accessing random pos suitable for vector?
    for(it = m_indexes.begin(); it != m_indexes.end()-2; it++ )    {

        // cout << *it << endl; 

        auto current = *it;
        auto next = *(it+1);

        if(next-current == r.step) {
            // cout << "same step: " << r.step << endl; 
            if(r.isSingleFrame) r.isSingleFrame = false;
            r.end = next;
            continue;
        }
        else {
            // cout << "diff step: " << r.step << endl; 
            m_ranges.push_back(r);
            // cout << "push range: "<< r.start << "-" << r.end << "-" << r.step << endl; 

            if(it+1 != m_indexes.end()){
                r.isSingleFrame = true;
                r.start = next;
                r.end = next;
                r.step = *(it+2) - next;
                // cout << "next range: "<< r.start << "-" << r.end << "-" << r.step << endl; 
            }
        }
    }

    // Handle last range and last item
    // TODO check if step consistent else push range without las tand create single last frame
    // r.end = m_indexes.back();
    // m_ranges.push_back(r);

    // cout << "after loop range: "<< r.start << "-" << r.end << "-" << r.step << endl; 

    // TOFIX accessing random pos suitable for vector?
    int last = m_indexes.back();
    int beforeLast = m_indexes.at(m_indexes.size()-2);

    cout << "beforeLast = " << beforeLast << endl;
    cout << "last = " << last << endl;

    if(last-r.end == r.step) {
        r.end = last;
        if(r.isSingleFrame) r.isSingleFrame = false;
        m_ranges.push_back(r);
        // cout << "push range: "<< r.start << "-" << r.end << "-" << r.step << endl; 
    }
    else{
        // r.step = last - beforeLast;
        m_ranges.push_back(r);
        // cout << "push range: "<< r.start << "-" << r.end << "-" << r.step << endl; 
        
        r.isSingleFrame = true;
        r.start = last;
        r.end = last;
        r.step = 0;

        m_ranges.push_back(r);
        // cout << "push range: "<< r.start << "-" << r.end << "-" << r.step << endl; 
    }
    // }


}