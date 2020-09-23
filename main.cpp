/***********************************************************************************************************************
 *
 * Josue Zamitiz
 *
 * CS2308
 *
 * Assignment 4
 *
***********************************************************************************************************************/

#include <iostream>
#include <string>
#include <fstream>
#include "Contact.h"

using namespace std;

void displayContacts(Contact*[], int*, bool);
void addContact(Contact**&, int*);
void searchContacts(Contact*[], int*);
void deleteContact(Contact**&, int*);
void loadDatabase(Contact**&, int*);
void saveToDatabase(Contact*[], int*);
void editContact(Contact**&, int*);

void editFirstName(Contact**&, int);
void editLastName(Contact**&, int);
void editPhoneNumber(Contact**&, int);
void editPhoneType(Contact**&, int);

string phoneNumberModifyString(string);
bool isEqual(Contact*, Contact*);
void sortContacts(Contact*[], int*);
void contactsMatchingSearchTerm(Contact*[], int*, string, Contact**&, int*);
void swap(Contact*&, Contact*&);
string toLower(string);
int indexOfContactInArray(Contact*[], int*, Contact*);
void addContactToArray(Contact*, Contact**&, int*);
void deleteContactFromArrayAtIndex(Contact**&, int*, int);

static const string DATABASE_TXT_FILE = "database.txt";
static const char DELIMITER = '~';

int main() {

    int* numberOfContacts = new int(0);
    Contact** contacts = new Contact*[*numberOfContacts];

    loadDatabase(contacts, numberOfContacts);

    int mainMenuChoice = 0;

    while (mainMenuChoice != 6) {

        cout << "\nSelect an operation...\n"
             << "1. Display all contacts.\n"
             << "2. Add new contact.\n"
             << "3. Search for contact.\n"
             << "4. Delete a contact.\n"
             << "5. Edit a Contact.\n"
             << "6. Exit.\n\n";

        cin >> mainMenuChoice;

        switch (mainMenuChoice) {

            case 1:
                displayContacts(contacts, numberOfContacts, false);
                break;

            case 2:
                addContact(contacts, numberOfContacts);
                break;

            case 3:
                searchContacts(contacts, numberOfContacts);
                break;

            case 4:
                deleteContact(contacts, numberOfContacts);
                break;

            case 5:
                editContact(contacts, numberOfContacts);

            case 6:
                saveToDatabase(contacts, numberOfContacts);
                break;

        }
    }

    return 0;
}

void displayContacts(Contact* contacts[], int* numberOfContacts, bool isNumbered) {

    for (int i = 0; i < *numberOfContacts; i++) {

        Contact* contact = contacts[i];

        string phoneTypeString;

        switch (contact->getPhoneType()) {
            case CELL:
                phoneTypeString = "Cell";
                break;
            case HOME:
                phoneTypeString = "Home";
                break;
            case WORK:
                phoneTypeString = "Work";
                break;
            case UNKNOWN:
                phoneTypeString = "Unknown";
                break;
        }

        string listNumber;
        if (isNumbered) {
            listNumber = to_string(i + 1) + ". ";
        } else {
            listNumber = "";
        }

        cout << endl << listNumber
             << contact->getFirstName() << " "
             << contact->getLastName() << ", "
             << phoneTypeString << " "
             << phoneNumberModifyString(contact->getPhoneNumber());
    }

    cout << endl;
}

void addContact(Contact**& contacts, int* numberOfContacts) {

    string firstName, lastName;

    Contact* contact = new Contact(firstName,lastName);

    cout << "\nFirst Name: ";
    cin >> firstName;
    contact->setFirstName(firstName);

    cout << "\nLast Name: ";
    cin >> lastName;
    contact->setLastName(lastName);

    bool isValidPhoneNumber = false;

    while (!isValidPhoneNumber) {

        string stringPhoneNumber;
        cout << "\n10 Digit Phone Number (digits only): ";
        cin >> stringPhoneNumber;

        isValidPhoneNumber = contact->setPhoneNumber(stringPhoneNumber);
    }

    int phoneTypeInt = 0;

    while (phoneTypeInt < 1 || phoneTypeInt > 4) {

        cout << "\nSelect phone type...\n"
             << "1. Cell\n"
             << "2. Home\n"
             << "3. Work\n"
             << "4. Unknown\n\n";

        cin >> phoneTypeInt;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (phoneTypeInt) {
            case 1:
                contact->setPhoneType(CELL);
                break;
            case 2:
                contact->setPhoneType(HOME);
                break;
            case 3:
                contact->setPhoneType(WORK);
                break;
            case 4:
                contact->setPhoneType(UNKNOWN);
                break;
        }
    }

    addContactToArray(contact, contacts, numberOfContacts);

    cout << "\nContact added.\n";
}

void searchContacts(Contact* contacts[], int* numberOfContacts) {

    cout << "\nEnter the last name of the contact: ";
    string searchTerm;
    cin >> searchTerm;

    int* numberOfMatchingContacts = new int(0);
    Contact** matchingContacts = new Contact*[*numberOfMatchingContacts];

    contactsMatchingSearchTerm(contacts, numberOfContacts, searchTerm, matchingContacts, numberOfMatchingContacts);

    if (*numberOfMatchingContacts > 0) {
        displayContacts(matchingContacts, numberOfMatchingContacts, false);
    } else {
        cout << "\nNo contacts matching the name: " << searchTerm << "." << endl;
    }

    delete[] matchingContacts;
    delete numberOfMatchingContacts;
}

void deleteContact(Contact**& contacts, int* numberOfContacts) {

    cout << "\nEnter the first or last name of the contact: ";
    string searchTerm;
    cin >> searchTerm;

    int* numberOfMatchingContacts = new int(0);
    Contact** matchingContacts = new Contact*[*numberOfMatchingContacts];

    contactsMatchingSearchTerm(contacts, numberOfContacts, searchTerm, matchingContacts, numberOfMatchingContacts);

    if (*numberOfMatchingContacts > 0) {

        cout << "\nSelect contact to delete...\n";
        displayContacts(matchingContacts, numberOfMatchingContacts, true);
        cout << endl;

        int selectionInt;
        cin >> selectionInt;

        Contact* contactSelectedForDelete = matchingContacts[selectionInt - 1];

        int indexToDelete = indexOfContactInArray(contacts, numberOfContacts, contactSelectedForDelete);

        deleteContactFromArrayAtIndex(contacts, numberOfContacts, indexToDelete);

        cout << "\nContact deleted.\n";

    } else {
        cout << "\nNo contacts matching the name: " << searchTerm << "." << endl;
    }

    delete[] matchingContacts;
    delete numberOfMatchingContacts;
}

void loadDatabase(Contact**& contacts, int* numberOfContacts) {

    ifstream fin;
    fin.open(DATABASE_TXT_FILE);

    string firstName;
    string lastName;
    string phoneNumberString;
    string phoneTypeString;

    //int index = 0;

    while (getline(fin, firstName, DELIMITER)) {

        getline(fin, lastName, DELIMITER);
        getline(fin, phoneNumberString, DELIMITER);
        getline(fin, phoneTypeString);

        Contact* contact = new Contact(firstName, lastName);

        contact->setFirstName(firstName);
        contact->setLastName(lastName);
        contact->setPhoneNumber(phoneNumberString);

        int phoneTypeInt = stoi(phoneTypeString);

        switch (phoneTypeInt) {
            case 0:
                contact->setPhoneType(CELL);
                break;
            case 1:
                contact->setPhoneType(HOME);
                break;
            case 2:
                contact->setPhoneType(WORK);
                break;
        }

        addContactToArray(contact, contacts, numberOfContacts);
        //index++;
    }

    fin.close();
}

void saveToDatabase(Contact* contacts[], int* numberOfContacts) {

    ofstream fout;
    fout.open(DATABASE_TXT_FILE);

    for (int i = 0; i <= (*numberOfContacts) - 1; i++) {

        Contact* contact = contacts[i];

        fout << contact->getFirstName() << "~"
             << contact->getLastName() << "~"
             << contact->getPhoneNumber() << "~"
             << contact->getPhoneType() << endl;
    }

    fout.close();
}

string phoneNumberModifyString(string phoneNumber) {

    string phoneNumberString = phoneNumber;

    string areaCode = phoneNumberString.substr(0, 3);
    string prefix = phoneNumberString.substr(3, 3);
    string suffix = phoneNumberString.substr(6, 4);

    return "(" + areaCode + ")" + prefix + "-" + suffix;
}

bool isEqual(Contact* a, Contact* b) {

    if (a->getLastName().compare(b->getLastName()) != 0) {
        return false;
    }

    if (a->getFirstName().compare(b->getFirstName()) != 0) {
        return false;
    }

    if (a->getPhoneNumber() != b->getPhoneNumber()) {
        return false;
    }

    if (a->getPhoneType() != b->getPhoneType()) {
        return false;
    }

    return true;
}

void sortContacts(Contact* contacts[], int* numberOfContacts) {

    int maxIndex;
    int index;

    for (maxIndex = (*numberOfContacts) - 1; maxIndex > 0; maxIndex--) {

        for (index = 0; index < maxIndex; index++) {

            if (toLower(contacts[index]->getLastName()) > toLower(contacts[index + 1]->getLastName())) {
                swap(contacts[index], contacts[index + 1]);
            }
        }
    }
}

void contactsMatchingSearchTerm(
        Contact* contacts[],
        int* numberOfContacts,
        string searchTerm,
        Contact**& results,
        int* numberOfResults) {

    *numberOfResults = 0;

    int first = 0;
    int middle;
    int last = (*numberOfContacts) - 1;

    while (first <= last) {

        middle = (first + last) / 2;

        if (toLower(contacts[middle]->getLastName()) == toLower(searchTerm)) {

            addContactToArray(contacts[middle], results, numberOfResults);

            bool didFind = true;
            int leftIndex = middle - 1;
            while (didFind && leftIndex >= 0) {
                if (toLower(contacts[leftIndex]->getLastName()) == toLower(searchTerm)) {
                    addContactToArray(contacts[leftIndex], results, numberOfResults);
                    leftIndex--;
                } else {
                    didFind = false;
                }
            }

            didFind = true;
            int rightIndex = middle + 1;
            while (didFind && rightIndex < (*numberOfContacts) - 1) {
                if (toLower(contacts[rightIndex]->getLastName()) == toLower(searchTerm)) {
                    addContactToArray(contacts[rightIndex], results, numberOfResults);
                    rightIndex++;
                } else {
                    didFind = false;
                }
            }

            return;

        } else if (toLower(searchTerm) > toLower(contacts[middle]->getLastName())) {
            first = middle + 1;
        } else {
            last = middle - 1;
        }
    }
}

int indexOfContactInArray(Contact* contacts[], int* numberOfContacts, Contact* contact) {

    int first = 0;
    int middle;
    int last = *numberOfContacts - 1;

    while (first <= last) {

        middle = (first + last) / 2;

        if (isEqual(contacts[middle], contact)) {
            return middle;
        } else if (contact->getLastName() > contacts[middle]->getLastName()) {
            first = middle + 1;
        } else {
            last = middle - 1;
        }
    }

    return -1;
}

void swap(Contact*& a, Contact*& b) {
    Contact* temp = a;
    a = b;
    b = temp;
}

string toLower(string a) {
    std::transform(a.begin(), a.end(), a.begin(), ::tolower);
    return a;
}

void addContactToArray(Contact* contact, Contact**& contacts, int* numberOfContacts) {

    int newNumberOfContacts = *numberOfContacts + 1;
    Contact** newContactsArray = new Contact*[newNumberOfContacts];

    for (int i = 0; i < (*numberOfContacts); i++) {
        newContactsArray[i] = contacts[i];
    }

    delete[] contacts;

    *numberOfContacts = newNumberOfContacts;
    newContactsArray[(*numberOfContacts) - 1] = contact;

    contacts = newContactsArray;

    sortContacts(contacts, numberOfContacts);
}

void deleteContactFromArrayAtIndex(Contact**& contacts, int* numberOfContacts, int indexToDelete) {

    int newNumberOfContacts = (*numberOfContacts) - 1;
    Contact** newContactsArray = new Contact*[newNumberOfContacts];

    int newIndex = 0;
    for (int i = 0; i < *numberOfContacts; i++) {
        if (i != indexToDelete) {
            newContactsArray[newIndex] = contacts[i];
            newIndex++;
        }
    }

    delete contacts[indexToDelete];
    delete[] contacts;
    *numberOfContacts = newNumberOfContacts;
    contacts = newContactsArray;

    sortContacts(contacts, numberOfContacts);

}

void editContact(Contact**& contacts, int* numberOfContacts){

    cout << "\nEnter the last name of the contact to Edit: \n";
    string searchTerm;
    cin >> searchTerm;

    int* numberOfMatchingContacts = new int(0);
    Contact** matchingContacts = new Contact*[*numberOfMatchingContacts];

    contactsMatchingSearchTerm(contacts, numberOfContacts, searchTerm, matchingContacts, numberOfMatchingContacts);

    if (*numberOfMatchingContacts > 0) {
            displayContacts(matchingContacts, numberOfMatchingContacts, true);

            int name_to_change, choice;

            cout << "Choose the number of the contact to edit: \n";
            cin >> name_to_change;

            Contact* contactSelectedForEdit = matchingContacts[name_to_change - 1];

            int indexToChange = indexOfContactInArray(contacts, numberOfContacts, contactSelectedForEdit);


            cout << "Choose an option to edit:\n"
                 << "1. First Name\n"
                 << "2. Last Name\n"
                 << "3. Phone Number\n"
                 << "4. Phone Type\n" << endl;
            cin >> choice;

            switch (choice) {
                case 1://edit firstName
                    editFirstName(contacts, indexToChange);
                    break;
                case 2://edit lastName
                    editLastName(contacts, indexToChange);
                    break;
                case 3:// edit phoneNumber
                    editPhoneNumber(contacts, indexToChange);
                    break;
                case 4://Phone type
                    editPhoneType(contacts, indexToChange);
                    break;
            }


        }

     else {
        cout << "\nNo contacts matching the name: " << searchTerm << "." << endl;
    }


    delete[] matchingContacts;
    delete numberOfMatchingContacts;

}

void editFirstName(Contact**& contacts, int indexToChange){

    string name_change;

    Contact* contact = contacts[indexToChange];

    cout << "Enter the new first name: \n";
    cin >> name_change;

    contact->setFirstName(name_change);

    cout << "Done!" << endl;
}

void editLastName(Contact**& contacts, int indexToChange){

    string name_change;

    Contact* contact = contacts[indexToChange];

    cout << "Enter the new last name: \n";
    cin >> name_change;

    contact->setLastName(name_change);

    cout << "Done!" << endl;

}

void editPhoneNumber(Contact**& contacts, int indexToChange){

    string phoneNumber_change;

    Contact* contact = contacts[indexToChange];

    bool isValidPhoneNumber = false;

    while (!isValidPhoneNumber) {

        cout << "Enter the new phone number: \n";
        cin >> phoneNumber_change;
        isValidPhoneNumber = contact->setPhoneNumber(phoneNumber_change);
    }

    cout << "Done!" << endl;
}

void editPhoneType(Contact**& contacts, int indexToChange){

    Contact* contact = contacts[indexToChange];

    int phoneType_change = 0;

    while (phoneType_change < 1 || phoneType_change > 4) {

        cout << "\nSelect phone type...\n"
             << "1. Cell\n"
             << "2. Home\n"
             << "3. Work\n"
             << "4. Unknown\n\n";

        cin >> phoneType_change;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (phoneType_change) {
            case 1:
                contact->setPhoneType(CELL);
                break;
            case 2:
                contact->setPhoneType(HOME);
                break;
            case 3:
                contact->setPhoneType(WORK);
                break;
            case 4:
                contact->setPhoneType(UNKNOWN);
                break;
        }
    }

    cout << "Done!" << endl;
}