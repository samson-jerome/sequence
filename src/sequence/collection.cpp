#include "collection.h"

using namespace sequence;

Collection::Collection(const sequence::Collection &c) {
    // m_indices.insert(c.m_indices.begin(), c.m_indices.end());
	// m_holes = c.m_holes;
    m_head = c.m_head;
    m_tail = c.m_tail;
	m_padding = c.m_padding;
    // m_ranges = c.m_ranges;
    this->_findHoles();
    this->_separate();

}

Collection::Collection(const std::string &head, const std::string &tail, const std::vector<int> &indices) 
    :m_head(head), m_tail(tail)
{
    m_indices = std::set<int> (indices.begin(), indices.end());
    this->_findHoles();
    this->_separate();
}

Collection::Collection(const std::string& head, const std::string& tail, const int start, const int end)
    :m_head(head), m_tail(tail)
{
    for (auto i = start; i <= end; ++i) {
        cout << i << " " << endl;
        m_indices.insert(i);
    }
    this->_findHoles();
    this->_separate();
}

Collection::Collection(const std::string &head,const std::string &tail, const std::vector<int> & indices, const int padding)
    :m_head(head), m_tail(tail), m_padding(padding)
{
    m_indices = std::set<int> (indices.begin(), indices.end());

    this->_findHoles();
    this->_separate();
}

Collection::Collection(const std::string& head, const std::string& tail, const int start, const int end, const int padding) {
    for (auto i = start; i <= end; ++i) {
        cout << i << " " << endl;
        m_indices.insert(i);
    }
    this->_findHoles();
    this->_separate();
}

int Collection::count() {
    return m_indices.size();
}

int Collection::first() {
	if (m_indices.empty()) {
		throw "Empty collection";
	}
    return *(m_indices.begin());
}

int Collection::last() {
    return *(m_indices.end());
}

// http://www.cplusplus.com/reference/set/set/
void Collection::insert(int frame) {

    int current;
    // for (std::vector<int>::const_iterator i = m_indexes.begin(); i != m_indexes.end(); ++i) {
    //     if *i < 
    // }

    // int prev = m_indexes.at(0);
    // int current;

    // for (std::vector<int>::const_iterator i = m_indexes.begin()+1; i != m_indexes.end(); ++i) {
    //     current = *i;
    //     while (current-prev != 1) {
    //         m_holes.push_back(++prev);
    //     }
    //     ++prev;
    // }

    // Recreate the ranges
    this->_separate();
}
void Collection::insert(std::vector<int> frames_list) {}

void Collection::remove(int frame) {}
void Collection::remove(std::vector<int> frames_list) {}
void Collection::update(int frame) {}
void Collection::update(std::vector<int> frames_list) {}


std::string Collection::head() const{
    return m_head;
}

void Collection::setHead(std::string head){
    m_head = head;
}

std::string Collection::tail() const{
    return m_tail;
}

void Collection::setTail(std::string tail){
    m_tail = tail;
}

int Collection::padding() const{
    return m_padding;
}

void Collection::setPadding(int padding) {
    if(0 < padding)
        m_padding = padding;
    throw std::invalid_argument("padding cannot be negative");
}

// TODO update to be similar to print from fmt taking an stream or none to use stdout
void Collection::info() {
    std::cout << "Collection construct" << std::endl;
    std::cout << "  head...: " << m_head << std::endl;
    std::cout << "  tail...: " << m_tail << std::endl;
    std::cout << "  padding: " << m_padding << std::endl;
    std::cout << "  indexes: " ;
    for (std::set<int>::const_iterator i = m_indices.begin(); i != m_indices.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
    std::cout << "  holes..: ";
    for (std::vector<int>::const_iterator i = m_holes.begin(); i != m_holes.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;
}

// void Collection::print(format=CollectionFormats.default) {

// }

// std::string Collection::format(const std::string &format) {
//     return format;
// }

/**
 * Formats returns a string with information about the current collection 
 * formatted using the desired pattern (default head.[1:10].ext)
 * \return a string
*/
std::string Collection::format() {
    std::string fullRange = "";

    for(Range r : m_ranges){
        if(r.isSingleFrame) {
            fullRange += fmt::format("{index:0>{padding}}", "index"_a=r.start,
                "padding"_a=m_padding) + ",";
        } else {
            std::string val;
            if(r.step == 1){
                val = fmt::format("{start:0>{padding}}:{end:0>{padding}}", "start"_a=r.start, "end"_a=r.end,
                    "padding"_a=m_padding);
            } else {
                val = fmt::format("{start:0>{padding}}:{end:0>{padding}}x{step}", "start"_a=r.start, "end"_a=r.end,
                    "step"_a=r.step, "padding"_a=m_padding);
            }
            // auto val = (r.step==1) ? 
            //     std::to_string(r.start) + ":" + std::to_string(r.end) :
            //     std::to_string(r.start) + ":" + std::to_string(r.end) + "x" + std::to_string(r.step);
            fullRange += val + ",";
        }
    }
    fullRange.erase(fullRange.size() - 1);

    return fmt::format("{head}[{range:0<{padding}}]{tail}",  
        "head"_a=m_head, "range"_a=fullRange, "tail"_a=m_tail,
        "padding"_a=m_padding);
}


/**
 * Use collection details to express each item name individually
 * \return the list of all expanded names
 */
std::vector<std::string> Collection::getItems() const{
    std::vector<std::string> result;
    for(auto i : m_indices){
        result.push_back(fmt::format("{head}{range:0>{padding}}{tail}",  
            "head"_a=m_head, "range"_a=i, "tail"_a=m_tail,
            "padding"_a=m_padding));
    }
    return result;
}

/**
 * Retrieve an individual name for a given index
 * \param integer value of the frame to retrieve
 * \return Pair of string and bool. The string holds the expanded name if 
 *  found or an empty string. The boolean indicate a successful search
 */
std::pair<std::string, bool> Collection::getItem(int frame) const{
    std::pair<std::string, bool> result;
    auto frame_it = m_indices.find(frame);

    if( frame_it != m_indices.end() ) {
        result.first = fmt::format("{head}{range:0>{padding}}{tail}",
            "head"_a = m_head, "range"_a = *(frame_it), "tail"_a = m_tail,
            "padding"_a = m_padding);
        result.second = true;
    }
    else {
        result.second = false;
    }
    return result;
}

/**
 * Retrieve an individual name given its position in frames list
 * \param integer value of the index in the collection
 * \return A pair of string and boolean. The string holds the expanded name if
 *  found or an empty string. The boolean indicate a successful search
 */
std::pair<std::string, bool> Collection::getNthItem(int index) const {
    std::pair<std::string, bool> result;
    if (m_indices.size() > index)
    {
        int frame = *(std::next(m_indices.begin(), index));
        result.first = fmt::format("{head}{range:0>{padding}}{tail}",
            "head"_a = m_head, "range"_a = frame, "tail"_a = m_tail,
            "padding"_a = m_padding);

        result.second = true;
    }
    else
        result.second = false;

    return result;
}


/**
Updates current collection to recreate the list of missing frames from the list 
of indices.
*/
void Collection::_findHoles() {

    // If length > 0
    m_holes.clear();

    int prev = *(m_indices.begin());
    int current;

    for (std::set<int>::const_iterator i = m_indices.begin()++; i != m_indices.end(); ++i) {
        current = *i;
        while (current-prev != 1) {
            m_holes.push_back(++prev);
        }
        ++prev;
    }
}

/**
Updates current collection to recreate the list of ranges from the list of 
indices.
*/
void Collection::_separate(int minimum_items){

    Range r;
    m_ranges.clear();

    if(m_indices.empty()) {
        return;
    } 

    else if (m_indices.size() == 1) {
        r.isSingleFrame = true;
        r.start = *(m_indices.begin());
        r.end = r.start;
        r.step = 1;

        m_ranges.push_back(r);
        return;
    }

    else if (m_indices.size() == 2) {
        // 2 indexes collection are always treated as 2 single frames if not consecutives
        // TODO if not consistent when more than 3 where list is: 1,2,8 for instance > considere it as a range
        r.isSingleFrame = true;
        r.start = *(m_indices.begin());
        r.end = r.start;
        r.step = 1;
        m_ranges.push_back(r);

        r.isSingleFrame = true;
        r.start = *(m_indices.begin()++);
        r.end = r.start;
        r.step = 1;
        
        m_ranges.push_back(r);
        return;
    }

    // ------------------------------------------------------------------------------------
    // Handle a vector of more than 2 items
    cout << "Separate more than 2 elems:" << endl;
    std::set<int>::iterator it;

    // Init first range
    r.isSingleFrame = true;
    r.start = *(m_indices.begin());
    r.end = *(m_indices.begin());
    r.step = *(m_indices.begin()++) - *(m_indices.begin());

    // TOFIX accessing random pos suitable for vector?
    auto it_end = std::next(m_indices.begin(), m_indices.size()-2);
    for(it = m_indices.begin(); it != it_end; it++ )    {

        cout << *it << endl; 

        auto current = *it;
        auto next = *(it++);

        if(next-current == r.step) {
            cout << "same step: " << r.step << endl; 
            if(r.isSingleFrame) r.isSingleFrame = false;
            r.end = next;
            continue;
        }
        else {
            cout << "diff step: " << r.step << endl; 
            m_ranges.push_back(r);
            cout << "push range: "<< r.start << "-" << r.end << "-" << r.step << endl; 

            // If we are sure the next element is not the last we can prep a
            // new range
            if(std::next(it) != m_indices.end()){
                r.isSingleFrame = true;
                r.start = next;
                r.end = next;
                r.step = *(std::next(it,2)) - next;
                cout << "next range: "<< r.start << "-" << r.end << "-" << r.step << endl; 
            }
        }
    }

    // Handle last range and last item
    // TODO check if step consistent else push range without las tand create single last frame
    // r.end = m_indexes.back();
    // m_ranges.push_back(r);

    // cout << "after loop range: "<< r.start << "-" << r.end << "-" << r.step << endl; 

    // TOFIX accessing random pos suitable for vector?
    int last = *(m_indices.end());
    int beforeLast = *(m_indices.rend()++);

    cout << "beforeLast = " << beforeLast << endl;
    cout << "last = " << last << endl;

    if(last-r.end == r.step) {
        r.end = last;
        if(r.isSingleFrame) r.isSingleFrame = false;
        m_ranges.push_back(r);
        cout << "push range: "<< r.start << "-" << r.end << "-" << r.step << endl; 
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