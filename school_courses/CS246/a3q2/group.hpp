//
//  group.hpp
//  a3q2
//
//  Created by Ethan on 2019-06-20.
//  Copyright © 2019 Ethan. All rights reserved.
//

#ifndef _GROUP_H
#define _GROUP_H
#include <string>

struct Group; // forward declaration

// An EmailNode implements the linked list of email addresses for a Group.
// It may be a singly- or doubly-linked list, the choice is up to you.
// Each node contains the email address as a string.
struct EmailNode {
    
    EmailNode( const std::string & email );
    ~EmailNode();
    
    // Add your instance variables and methods here.
    std::string address;
    EmailNode *nexte;
    
};

// A GroupNode implements the linked list of sub-groups for a Group.
// It may be a singly- or doubly-linked list, the choice is up to you.
struct GroupNode {
    GroupNode( Group * group );
    ~GroupNode();
    
    // Add your instance variables and methods here.
    Group *sub;
    GroupNode *nexts;
        
};

// A Group contains the name of the group, a list of email addresses, and a
// list of sub-groups. It is possible that either list, or both, may be empty.
struct Group {
    
    std::string groupName;
    EmailNode *firste;
    GroupNode *firsts;
    
    Group( const std::string & name );
    ~Group();
    
    // Return the group's name.
    std::string name() const;
    
    // Return a string containing all of the email addresses in the group. The
    // format of the string for a group that contains 3 email addresses,
    // {e1,e2,e3} is: "[e1, e2, e3]"
    std::string emails() const;
    
    // Add the given group as a subgroup to this one. Requires that nobody else
    // have the address to this group in order to prevent cycles and multiple
    // memory frees. Groups are stored in standard string sort order by their
    // names. If there are subgroups with the same name, this one is added
    // after all of the subgroups with the same name.
    void addGroup( Group * group );
    
    // Add the given email address to list of email addresses for the current
    // group. Addresses are stored in standard string sort order. If there are
    // email addresses that are the same as the one being added, the new one
    // is placed after the last one it matches.
    void addAddress( const std::string & name );
    
    // Remove the first occurrence of the subgroup with that name from the list
    // of subgroups. Returns true if successful, otherwise returns false.
    bool removeGroup( const std::string & name );
    
    // Remove the first occurrence of the email address from the list of email
    // addresses. Returns true if successful, otherwise returns false.
    bool removeAddress( const std::string & name );
    
    // Return the address of the first node containing the subgroup with the
    // specified name or nullptr if not found. The search stops as soon as the
    // first occurrence is found. If the group name cannot be found in the
    // current Group's list of subgroups, then each subgroup in turn is
    // searched. Since each subgroup is a Group, the search action follows the
    // previously specified order.
    GroupNode * findGroup( const std::string & name );
    
    // Return the address of the first node containing the subgroup with the
    // specified email address or nullptr if not found. Searching for an email
    // address first starts in the list of email addresses for current Group.
    // The search stops as soon as the first occurrence is found. If the
    // address cannot be found in this Group's email address list, then each
    // subgroup in turn is searched. Since each subgroup is a Group, the search
    // action follows the previously specified order.
    EmailNode * findAddress( const std::string & name );
    
    // Add your instance variables and helper methods here.
    // ***
};


//print the nth dimension group
void printGroup(std::ostream &out, Group & group, int n);


// Outputs the information for a group. First the name of the group is output,
// followed by a newline. Then the list of email addresses for the group is
// output on the following line. Then each subgroup's information is output. In
// particular, the information of every subgroup is indented using tabs to help
// visually indicate what is a subgroup. See the provided sample.{in,out} files
// and the sample executable.
std::ostream & operator<<(std::ostream &, Group & group );

// You may add output operators for EmailNode and/or GroupNode if you wish.
std::ostream & operator<<(std::ostream &out, GroupNode & groupnode );

std::ostream & operator<<(std::ostream &out, EmailNode & emailnode );

#endif
