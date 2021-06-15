//
//  main.cpp
//  a3q2
//
//  Created by Ethan on 2019-06-20.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "group.hpp"
#include <iostream>
#include <string>
using namespace std;

// The test harness allows you to have up to 10 groups defined at one time,
// identified as "g0" to "g9". If a group has not been initialized, it consists
// of a nullptr. Most of the test harness commands cannot be performed upon an
// uninitialized group.
//
// Additionally, the user prompts are printed to standard error so that they
// will not interfere with the output produced, and thus make it easier to
// write your test files.
//
// Command        Description
// b g_i name     Initializes group g_i by calling its constructor and passing
//                in the group name, name. g_i must initially be a nullptr.
// aa g_i email   Uses Group::addAddress to add email to g_i. g_i must not be
//                a nullptr.
// ag g_i g_j     Uses Group::addGroup to add g_j to g_i and sets g_j to
//                nullptr. Neither g_i nor g_j must be a nullptr. This means
//                that you'll no longer be able to add email address to group
//                g_j, so you'll have to add all email addresses to it before
//                you make it a subgroup.
// ra g_i email   Uses Group::removeAddress to remove the first occurrence of
//                email from g_i. g_i must not be a nullptr.
// rg g_i name    Uses Group::removeGroup to remove the first subgroup of g_i
//                that has a name that matches "name". g_i must not be a
//                nullptr. A Group can only be deleted if it is a subgroup of
//                the Group currently being indexed; otherwise, the command
//                fails by doing nothing. For example, if the group g_0 is the
//                pointer to the WiCS group as in the question's diagram, then
//                the WiCS group will not be removed from g_0.
// sa g_i email   Uses Group::findAddress to return an (EmailNode*) set to the
//                node that contains the first occurrence of email in g_i or
//                nullptr if no such address can be found. g_i must not be a
//                nullptr.
// sg  g_i name   Uses Group::findGroup to return a (GroupNode*) set to the
//                node that contains the first occurrence of name in g_i as a
//                subgroup or nullptr if no such subgroup can be found. g_i
//                must not be a nullptr.
// p g_i          Uses operator<< to output group g_i to standard output. g_i
//                must not be a nullptr.

//************************************************************************
//  Helper variables and functions for test harness
//************************************************************************

//  test-harness operators
enum Op {NONE, BUILD_GROUP, ADD_ADDRESS, ADD_GROUP, REMOVE_ADDRESS, REMOVE_GROUP,
    SEARCH_GROUP, SEARCH_ADDRESS, PRINT_GROUP};

// converts a one-character input comment into its corresponding test-harness operator
Op convertOp( const string & opStr) {
    switch ( opStr[0] ) {
        case 'b':
            if ( opStr.length() == 1 ) return BUILD_GROUP;
        case 'a':
            if ( opStr.length() == 2 && opStr[1] == 'g' ) return ADD_GROUP;
            if ( opStr.length() == 2 && opStr[1] == 'a' ) return ADD_ADDRESS;
        case 'r':
            if ( opStr.length() == 2 && opStr[1] == 'g' ) return REMOVE_GROUP;
            if ( opStr.length() == 2 && opStr[1] == 'a' ) return REMOVE_ADDRESS;
        case 's':
            if ( opStr.length() == 2 && opStr[1] == 'g' ) return SEARCH_GROUP;
            if ( opStr.length() == 2 && opStr[1] == 'a' ) return SEARCH_ADDRESS;
        case 'p':
            if ( opStr.length() == 1 ) return PRINT_GROUP;
        default:
            return NONE;
    } // switch
} // convertOp

// Clears the cin error flags if invalid input is READ. Discards any input to the end of the line.
void fixcin() {
    cin.clear();
    string discard;
    getline(cin, discard);
} // fixcin

// Reads a Group id (g#) from cin.
// If the value read has a valid format (g#, where 0 <= g <= 9), the parameter is set to the number read.
// The return value indicates whether the read value has a valid format or not.
bool readName( int & index ) {
    const string errmsg = "Invalid name of Group variable.";
    char g;
    cin >> g;
    if ( g != 'g' ) {
        fixcin();
        cerr << errmsg << endl;
        return false;
    } // if
    
    index = -1;
    cin >> index;
    if ( index < 0 || index > 9 ) {
        fixcin();
        cerr << errmsg << endl;
        return false;
    } // if
    return true;
} // readName

// Read a string argument, either an email address or a group name.
// The return value indicates whether managed to obtain a value or not.
bool readString( string & s ) {
    cin >> s;
    if ( cin.fail() ) {
        fixcin();
        cerr << "Invalid string." << endl;
        return false;
    } // if
    return true;
} // readString

// Returns true if the pointer is a nullptr as required.
bool pointerMustBeNull( Group * ptr, int index ) {
    if ( ptr != nullptr ) {
        cerr << "Group g" << index << " is initialized and needs to be a nullptr." << endl;
        fixcin();
        return false;
    } // if
    return true;
} // pointerMustBeNull

// Returns true if the pointer is a nullptr as required.
bool pointerMustNotBeNull( Group * ptr, int index ) {
    if ( ptr == nullptr ) {
        cerr << "Group g" << index << " is not initialized and needs to not be a nullptr." << endl;
        fixcin();
        return false;
    } // if
    return true;
} // pointerMustNotBeNull

// Reads in a Group index as g#.
bool readGroupIndex( int & index, Group * groups[] ) {
    if ( ! readName( index ) ) return false;
    return true;
} // readGroupIndex

// Create a Group using the constructor
void buildGroup( Group * groups[] ) {
    int index;
    string name;
    if ( readName( index ) && readString( name ) && pointerMustBeNull( groups[index], index ) ) {
        groups[index] = new Group( name );
        cout << "Group g" << index << " with name \"" << name << "\" has been created." << endl;
    } // if
} // buildGroup

// Use operator<< to output the Group.
void printGroup( Group * groups[] ) {
    int index;
    if ( readGroupIndex( index, groups ) && pointerMustNotBeNull( groups[index], index ) ) {
        cout << "Group g" << index << " =\n" << *(groups[index]) << endl;
    } // if
} // printGroup

// Takes the group in the second index, and adds it to the group in the first index.
// Sets the group pointer at the second index to nullptr.
void addGroup( Group * groups[] ) {
    int index1, index2;
    if ( readGroupIndex( index1, groups ) && pointerMustNotBeNull( groups[index1], index1 ) &&
        readGroupIndex( index2, groups ) && pointerMustNotBeNull( groups[index2], index2 ) )
    {
        groups[index1]->addGroup( groups[index2] );
        cout << "Move g" << index2 << " to g" << index1 << endl;
        cout << "g" << index1 << " =\n"  << *(groups[index1]) << endl;
        groups[index2] = nullptr;
    } // if
} // addGroup

// Add an email address to the specified group.
void addAddress( Group * groups[] ) {
    int index;
    string address;
    if ( readGroupIndex( index, groups ) && readString( address ) &&
        pointerMustNotBeNull( groups[index], index ) ) {
        groups[index]->addAddress( address );
        cout << "Added email address \"" << address << "\" to " << "group g" << index << " =\n"
        << *(groups[index]) << endl;
    } // if
} // addAddress

// Remove an email address from the specified group.
void removeAddress( Group * groups[] ) {
    int index;
    string address;
    if ( readGroupIndex( index, groups ) && readString( address ) &&
        pointerMustNotBeNull( groups[index], index ) ) {
        bool result = groups[index]->removeAddress( address );
        cout << "Removed email address \"" << address << "\" from " << "group g" << index << " ";
        if ( ! result ) cout << "un";
        cout << "successfully; g" << index << " =\n" << *(groups[index]) << endl;
    } // if
} // removeAddress

// Remove the specified subgroup from the specified group.
void removeGroup( Group * groups[] ) {
    int index;
    string name;
    if ( readGroupIndex( index, groups ) && readString( name ) &&
        pointerMustNotBeNull( groups[index], index ) ) {
        bool result = groups[index]->removeGroup( name );
        cout << "Removed subgroup named \"" << name << "\" from " << "group g" << index << " ";
        if ( ! result ) cout << "un";
        cout << "successfully; g" << index << " =\n" << *(groups[index]) << endl;
    } // if
} // removeGroup

// Search the specified group for the specified email address.
void findAddress( Group * groups[] ) {
    int index;
    string address;
    if ( readGroupIndex( index, groups ) && readString( address ) &&
        pointerMustNotBeNull( groups[index], index ) )
    {
        EmailNode * node = groups[index]->findAddress( address );
        cout << "Find email address \"" << address << "\" in " << "group g" << index << " ";
        if ( node == nullptr )
            cout << "unsuccessfully" << endl;
        else
            cout << "successfully; node =\n" << *(node) << endl;
        node = nullptr; // don't want to delete found node
    } // if
} // findAddress

// Search the specified group for the specified group name.
void findGroup( Group * groups[] ) {
    int index;
    string name;
    if ( readGroupIndex( index, groups ) && readString( name ) &&
        pointerMustNotBeNull( groups[index], index ) )
    {
        GroupNode * node = groups[index]->findGroup( name );
        cout << "Find group name \"" << name << "\" in " << "group g" << index << " ";
        if ( node == nullptr )
            cout << "unsuccessfully" << endl;
        else
            cout << "successfully; node =\n" << *(node) << endl;
        node = nullptr; // don't want to delete found node
    } // if
} // findGroup

int main () {
    Group *groups[10] = {nullptr};
    
    for (;;) {
        string command;
        cin >> command;
        
        if ( cin.eof() ) break;
        
        Op op = convertOp(command);
        
        switch( op ) {
            case BUILD_GROUP:
                buildGroup( groups );
                break;
            case ADD_GROUP:
                addGroup( groups );
                break;
            case ADD_ADDRESS:
                addAddress( groups );
                break;
            case REMOVE_GROUP:
                removeGroup( groups );
                break;
            case REMOVE_ADDRESS:
                removeAddress( groups );
                break;
            case SEARCH_GROUP:
                findGroup( groups );
                break;
            case SEARCH_ADDRESS:
                findAddress( groups );
                break;
            case PRINT_GROUP:
                printGroup( groups );
                break;
            default:
                break;
        } // switch
    } // for
    
    for ( int i = 0 ; i < 10 ; i++ ) delete groups[i];
} // main

