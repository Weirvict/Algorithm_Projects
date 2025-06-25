# include <iostream>
#include <algorithm>
#include <list>

using namespace std;
/**
 *
 * @author Victoria Weir
 * @class CS4412
 * @date 9-21-23
 * @details Project 2 - Hashing
 *
 */

// Forward declarations
class Address; // Encapsulates the street number, street name, city, state, and zip code.
class AreaCode; // Represents the area code of a phone number.
class CountryCode; // Represents the country code of the phone number.
class Dictionary; // Contains a “hashtable” array of references to instances of the PhoneEntry class
class ReverseDictionary; // Reverse phone directory into the same set of phone entry class instances by hashing the ascii value of the phone numbers (country, area, phone number) into an unsigned integer representing the number of buckets.
class City; // Represents the city.
class Name; // Handling first names, middle names, and last names.
class Phone; // Reference to an instance of a Name class, an Address Class, and a PhoneNum Class.
class PhoneEntry; // Encapsulates a person's name, address, and phone number
class PhoneNumber; // Represents a phone number, including the area code and the phone number itself.
class State; // Represents the state.
class StreetName; // Represents the name of a street.
class StreetNum; // Represents the street number.
class Zip; // Represents the zip code.


/**
 * @details:
 * Had to rearrange the code since Address was getting the error of not getting the other classes first. Apologies for it not being alphabetical order.
 * A lot of the code contains getters and calls one another as seen in the class diagram. The class diagram is probably not as accurate as the code since I did not update it once I finished writing the code out.
 * Another thing to not is that I did not test every feature here in the project, since it would take a long time and would be repetitive since a lot of the code is similar to each other.
 * The purpose of the code is to use the hashtable and organize the multiple classes. It is probably not the best organization and code but it does work and could always be rewritten to be better.
 *
 */


// AreaCode Class
class AreaCode {
private:
    string areaCode;
public:
    AreaCode(const string& code) : areaCode(code) {}
    string getAreaCode() const {return areaCode; }
};

// City Class
class City {
private:
    string cityName;
public:
    City(const string& city) : cityName(city) {}
    string getCity() const { return cityName; }
};

class CountryCode{
private:string countryCode;
public:
    CountryCode(const string& code) : countryCode(code){}
    string getCountryCode() const { return countryCode;}
};

/**
 *
 * @class Name
 * @details This class gets the first, last, middle, initials, and full name of the person. As well as canonicalize the names.
 *
 */
class Name {
private:
    string firstName, middleName, lastName;
public:

    /**
     *
     * Constructor function
     * @param first Initializes First Name
     * @param middle Initializes Middle Name
     * @param last Initializes Last Name
     * Calls the canonicalize() function to format the names
     */
    Name(const string& first, const string& middle, const string& last)
            : firstName(first), middleName(middle), lastName(last) {
        canonicalize();
    }
    // Getters
    string getName(){
        return lastName + middleName + firstName;
    }
    string getFML(){
        return firstName + middleName + lastName;
    }
    string getInitials(){
        return lastName.substr(0, 1) + middleName.substr(0, 1) + firstName.substr(0, 1);
    }
    string getLastName(){
        return lastName;
    }
    string getFirstName() const {
        return firstName;
    }

    // Purpose is to ensure that the names are in a consistent and expected format as per the requirement of the asignment
    void canonicalize(){
        // Remove leading and trailing spaces
        firstName.erase(firstName.begin(), find_if(firstName.begin(), firstName.end(), [](char c) { return !isspace(c); }));
        firstName.erase(find_if(firstName.rbegin(), firstName.rend(), [](char c) { return !isspace(c); }).base(), firstName.end());

        middleName.erase(middleName.begin(), find_if(middleName.begin(), middleName.end(), [](char c) { return !isspace(c); }));
        middleName.erase(find_if(middleName.rbegin(), middleName.rend(), [](char c) { return !isspace(c); }).base(), middleName.end());

        lastName.erase(lastName.begin(), find_if(lastName.begin(), lastName.end(), [](char c) { return !isspace(c); }));
        lastName.erase(find_if(lastName.rbegin(), lastName.rend(), [](char c) { return !isspace(c); }).base(), lastName.end());

        // Uppercase first character, lowercase the rest
        if (!firstName.empty())
            firstName[0] = toupper(firstName[0]);
        transform(firstName.begin() + 1, firstName.end(), firstName.begin() + 1, ::tolower);

        if (!middleName.empty())
            middleName[0] = toupper(middleName[0]);
        transform(middleName.begin() + 1, middleName.end(), middleName.begin() + 1, ::tolower);

        if (!lastName.empty())
            lastName[0] = toupper(lastName[0]);
        transform(lastName.begin() + 1, lastName.end(), lastName.begin() + 1, ::tolower);
    }
};

// PhoneNumber Class
class PhoneNumber {
private:
    CountryCode countryCode;
    AreaCode areaCode;
    string number;
public:
    /**
     *
     * @param cc represents the country code of the phone number.
     * @param ac represents the area code of the phone number.
     * @param num represents the main part of the phone number.
     */
    PhoneNumber(const CountryCode& cc, const AreaCode& ac, const string& num)
            : countryCode(cc), areaCode(ac), number(num) {}

    string getPhoneNumber() const { return number; }
    string getFullPhoneNumber() const {return countryCode.getCountryCode() +"-"+ areaCode.getAreaCode() +"-"+ number; }
};

// State Class
class State {
private:
    string stateName;
public:
    State(const string& state) : stateName(state) {}
    string getState() const { return stateName; }
};

// StreetName Class
class StreetName {
private:
    string name;
public:
    StreetName(const string& n) : name(n) {}
    string getName() const { return name; }
};

// StreetNum Class
class StreetNum {
private:
    // Chose type basic_string<char>, which allows for more flexibility in representing street numbers.
    basic_string<char> number;
public:
    StreetNum(basic_string<char> num) : number(num) {}
    const basic_string<char> getNumber() const {return number; }
};

// Zip Class
class Zip{
private:
    string zipCode;
public:
    string getZip() const{return zipCode;}
    Zip(const string& zip) : zipCode(zip) {}
};

// Address Class
class Address {
private:
    StreetNum streetNum;
    StreetName streetName;
    City city;
    State state;
    Zip zip;

public:
    /**
     * Initializing the members of the Address class
     * @param num - The streetNum member is initialized with the StreetNum object num.
     * @param name - The streetName member is initialized with the StreetName object name.
     * @param city1 - The city member is initialized with the City object city1.
     * @param state1 - The state member is initialized with the State object state1.
     * @param zip1 - The zip member is initialized with the Zip object zip1.
     */
    Address(const StreetNum& num, const StreetName& name, const City& city1, const State& state1, const Zip& zip1)
            : streetNum(num), streetName(name), city(city1), state(state1), zip(zip1) {}

    string getDisplayString() const {
        return streetNum.getNumber() + " " + streetName.getName() + ", " + city.getCity() + ", " + state.getState() + " " + zip.getZip();
    }
};

// PhoneEntry Class
class PhoneEntry{
private:
    Name name;
    Address address;
    PhoneNumber phoneNumber;

public:
    /**
     * Initializing the members of the PhoneEntry class
     * @param first - Represents first name
     * @param middle - Represents middle name
     * @param last - Represents last name
     * @param streetnum - Represents street number
     * @param street - Represents street name
     * @param city - Represents city
     * @param state - Represents state
     * @param zip - Represents zip
     * @param phone - Represents phone
     */
    PhoneEntry(const string& first, const string& middle, const string& last, const string& streetnum, const string& street, const string& city,
               const string& state, const string& zip, const string& phone)
            : name(first, middle, last), address(StreetNum(streetnum), StreetName(street), City(city), State(state), Zip(zip)),
            phoneNumber(CountryCode(""),AreaCode(""), phone) {}

    string getDisplayString(){
        return name.getName() + " - " + phoneNumber.getPhoneNumber() + "\n" + address.getDisplayString();
    }

    //Getters
    string getInitials(){
        return name.getInitials();
    }
    string getFullName() {
        return name.getName();
    }
    string getFirstName() const {
        return name.getFirstName();
    }
    string getLastName(){
        return name.getLastName();
    }
    string getFML(){
        return name.getFML();
    }
    string getPhoneNumber(){
        return phoneNumber.getPhoneNumber();
    }
};

// Phone Class
class Phone{
private:
    Name name;
    Address address;
    PhoneNumber phoneNumber;
public:
    string getName = name.getName();
    string getAddress = address.getDisplayString();
    string phoneNum = phoneNumber.getFullPhoneNumber();

    string getDisplayString(){
        return getName + " - " + phoneNum + "\n" + address.getDisplayString();
    }
};

class Dictionary {
private:
    static const int bucketNum = 8;
    list<PhoneEntry> buckets[bucketNum]; // Use an array of lists

    // Hash function that accepts the string containing the lastName+MiddleInitial+FirstName and returns an integer between 0 and 7
    int hash(const string &key) const {
        // Calculate a hash value based on the key (e.g., full name)
        size_t hashValue = 0;
        for (char index : key) {
            // Simple hash function using character ASCII values
            hashValue += index;
        }
        // Use modulo division to map the hash value to a bucket index
        return hashValue % bucketNum;
    }

public:
    /**
     * Insert function - Inserting a PhoneEntry object into the appropriate bucket in the dictionary.
     * @param info - Represents the object which is being inserted into the dictionary
     */
    void insert(PhoneEntry info) {
        int bucketIndex = hash(info.getFullName());
        buckets[bucketIndex].push_back(info);
    };
    /**
     * Delete function - Deleting a PhoneEntry object from the dictionary based on the person's full name
     * @param fullName - Represents the full name of the person to be removed
     */
    void remove(const string &fullName) {
        int bucketIndex = hash(fullName);
        auto &bucket = buckets[bucketIndex];
        auto info = find_if(bucket.begin(), bucket.end(), [&fullName](PhoneEntry ex) {
            return ex.getFullName() == fullName;
        });
        if (info != bucket.end()) {
            bucket.erase(info);
        }
    }

    /**
     * Fetch Methods
     * Different types of ways to fetch or search for a persons name
     */
    list<PhoneEntry> fetchByLastName(const string &lastName) {
        int bucketIndex = hash(lastName);
        const auto &bucket = buckets[bucketIndex];

        list<PhoneEntry> results;
        for (PhoneEntry entry: bucket) {
            if (entry.getLastName() == lastName) {
                results.push_back(entry);
            }
        }
        return results;
    }
    list<PhoneEntry> fetchByFirstName(const string& firstName) {
        int bucketIndex = hash(firstName);
        const auto& bucket = buckets[bucketIndex];

        list<PhoneEntry> results;
        for (const auto& entry : bucket) {
            if (entry.getFirstName() == firstName) {
                results.push_back(entry);
            }
        }
        return results;
    }
    list<PhoneEntry> fetchByFullName(const string& fullName) {
        int bucketIndex = hash(fullName);
        const auto& bucket = buckets[bucketIndex];

        list<PhoneEntry> results;
        for (PhoneEntry entry : bucket) {
            if (entry.getFullName() == fullName) {
                results.push_back(entry);
            }
        }
        return results;
    }

    // getDisplayString method shows the list of entries for each of the 8 buckets
    void getDisplayString() const {
        for (int i = 0; i < bucketNum; ++i) {
            cout << "Bucket " << i << ":" << endl;
            for (PhoneEntry entry : buckets[i]) {
                cout << entry.getDisplayString() << endl;
            }
        }
    }
};

// Essentially the opposite of the Dictionary class but has a lot of similarities
class ReverseDictionary {
private:
    static const int bucketNum = 8; // Adjust the number of buckets as needed
    list<PhoneEntry> buckets[bucketNum]; // Use an array of lists

    // An unsigned integer representing the number of buckets
    unsigned int hashPhoneNumber(const string& phoneNumber) const {
        // Calculate a hash value based on the ASCII values of the phone number digits
        unsigned int hashValue = 0;
        for (char value : phoneNumber) {
            hashValue += (value);
        }
        // Use modulo division to map the hash value to a bucket index
        return hashValue % bucketNum;
    }

public:
    // Similar to the Dictionary class's insert, search, and display
    void insert(PhoneEntry entry) {
        // Hash the phone number and insert the entry into the corresponding bucket
        int bucketIndex = hashPhoneNumber(entry.getPhoneNumber());
        buckets[bucketIndex].push_back(entry);
    }

    // Search for phone entries by phone number
    list<PhoneEntry> searchByPhoneNumber(const string& phoneNumber) const {
        int bucketIndex = hashPhoneNumber(phoneNumber);
        const auto& bucket = buckets[bucketIndex];

        list<PhoneEntry> results;
        for (PhoneEntry entry : bucket) {
            if (entry.getPhoneNumber() == phoneNumber) {
                results.push_back(entry);
            }
        }
        return results;
    }

    // Display reverse phone directory entries
    void getDisplayString() const {
        for (int i = 0; i < bucketNum; ++i) {
            cout << "Bucket " << i << ":" << endl;
            for (PhoneEntry entry : buckets[i]) {
                cout << entry.getDisplayString() << endl;
            }
        }
    }
};

int main() {

    // Create instances of Dictionary and ReverseDictionary
    Dictionary phoneDictionary;
    ReverseDictionary reversePhoneDictionary;

    // Create and insert PhoneEntry objects into Dictionary
    PhoneEntry entry1("John", "Joe", "Doe", "123", "Main St", "New York", "NY", "10001", "1-123-456-7890");
    PhoneEntry entry2("Alice", "Ann", "Smith", "456", "Elm St", "Los Angeles", "CA", "90001", "2-987-654-3210");

    phoneDictionary.insert(entry1);
    phoneDictionary.insert(entry2);

    // Create and insert PhoneEntry objects into ReverseDictionary
    PhoneEntry entry3("Bob", "Dave", "Johnson", "789", "Oak St", "Chicago", "IL", "60601", "3-555-123-4567");
    PhoneEntry entry4("Eve", "Adam", "Williams", "101", "Pine St", "San Francisco", "CA", "94101", "1-777-888-9999");

    reversePhoneDictionary.insert(entry3);
    reversePhoneDictionary.insert(entry4);

    // Print a message to verify successful insertions
    cout << "Phone entries inserted successfully." << endl;

    // Testing Dictionary
    cout << "Fetching by last name 'Doe':" << endl;
    list<PhoneEntry> entriesByLastName = phoneDictionary.fetchByLastName("Doe");
    for (PhoneEntry entry: entriesByLastName) {
        cout << entry.getDisplayString() << endl;
    }
    // Print out the buckets
    //phoneDictionary.getDisplayString();

    // Testing ReverseDictionary
    cout << "Searching by phone number '3-555-123-4567':" << endl;
    list<PhoneEntry> entriesByPhoneNumber = reversePhoneDictionary.searchByPhoneNumber("3-555-123-4567");
    for (PhoneEntry entry: entriesByPhoneNumber) {
        cout << entry.getDisplayString() << endl;
    }
    // Print out the buckets
    //reversePhoneDictionary.getDisplayString();

    // Testing other functions
    //cout << "Initials:" << "\n" << entry1.getInitials() << "\n";
    //cout << "Phone: " << "\n" << entry2.getPhoneNumber() << "\n";

    return 0; // Exit the program
}
