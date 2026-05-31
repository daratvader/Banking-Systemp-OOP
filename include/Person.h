#pragma once
#include <string>

class Person {
protected:
    std::string firstName;
    std::string lastName;
private:
    std::string egn;
    std::string address;
    std::string phone;

public:
    Person(const std::string& fn, const std::string& ln,
           const std::string& egn, const std::string& addr,
           const std::string& phone);
    virtual ~Person() = default;

    // Accessors
    virtual std::string getFullName() const;
    std::string getEgn()     const { return egn; }
    // Returns the EGN with all but the last 4 digits masked (privacy).
    std::string getMaskedEgn() const;
    std::string getAddress() const { return address; }
    std::string getPhone()   const { return phone; }

    // Mutators (address and phone may change; EGN is immutable)
    void setAddress(const std::string& addr)  { address = addr; }
    void setPhone(const std::string& ph)      { phone   = ph;   }

    virtual void printInfo() const;
};
