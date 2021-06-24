//
//  group.cpp
//  a3q2
//
//  Created by Ethan on 2019-06-20.
//  Copyright © 2019 Ethan. All rights reserved.
//

#include "group.hpp"
#include <iostream>

// EmailNode constructors and methods
EmailNode::EmailNode( const std::string & email ) {
    address = std::string(email);
    nexte = nullptr;
}

EmailNode::~EmailNode() {
    delete nexte;
}


// GroupNode constructors and methods
GroupNode::GroupNode( Group * group ) {
    sub = group;
    nexts = nullptr;
}

GroupNode::~GroupNode() {
    delete sub;
    delete nexts;
}




// Group constructors and methods
Group::Group( const std::string & name ) {
    groupName = name;
    firste = nullptr;
    firsts = nullptr;
}

Group::~Group() {
    delete firste;
    delete firsts;
}


std::string Group::name() const {
    return groupName;
}


std::string Group::emails() const {
    EmailNode *curr = firste;
    std::string out = "[";
    while ( curr ) {
        out = out + curr->address;
        curr = curr->nexte;
        if ( curr ) {
            out = out + ", ";
        }
    }
    out = out + "]";
    return out;
}


void Group::addGroup( Group *group ) {
    GroupNode *newg = new GroupNode( group );
    GroupNode *curr = firsts;
    newg->nexts = firsts;
    if ( !firsts ) {
        firsts = newg;
    } else if ( firsts->sub->name() > newg->sub->name()) {
        firsts = newg;
    } else {
        GroupNode *prev = curr;
        curr = curr->nexts;
        while ( curr ) {
            if ( curr->sub->name() > newg->sub->name() ) break;
            prev = curr;
            curr = curr->nexts;
        }
        prev->nexts = newg;
        newg->nexts = curr;
    }
}
    
    
void Group::addAddress( const std::string &name ) {
    EmailNode *newe = new EmailNode( name );
    EmailNode *curr = firste;
    newe->nexte = firste;
    if ( !firste ) {
        firste = newe;
    } else if ( firste->address > newe->address ) {
        firste = newe;
    } else {
        EmailNode *prev = curr;
        curr = curr->nexte;
        while ( curr ) {
            if ( curr->address > newe->address ) break;
            prev = curr;
            curr = curr->nexte;
        }
        prev->nexte = newe;
        newe->nexte = curr;
    }
}


bool Group::removeGroup( const std::string &name ) {
    if ( !firsts ) {
        return false;
    } else {
        GroupNode *prev = firsts;
        GroupNode *curr = prev->nexts;
        if ( prev->sub->name() == name ) {
            firsts = curr;
            prev->nexts = nullptr;
            delete prev;
            return true;
        } else {
            while ( !prev->sub->removeGroup( name )) {
                if ( !curr ) {
                    return false;
                }
                if ( curr->sub->name() == name ) {
                    prev->nexts = curr->nexts;
                    curr->nexts = nullptr;
                    delete curr;
                    return true;
                } else {
                    prev = curr;
                    curr = curr->nexts;
                }
            }
        }
    }
    return false;
}


bool Group::removeAddress( const std::string &name ) {
    EmailNode *curr = firste;
    if ( curr ) {
        if ( curr->address == name ) {
            firste = curr->nexte;
            curr->nexte = nullptr;
            delete curr;
            return true;
        }
        while ( curr->nexte ) {
            if ( curr->nexte->address == name ) {
                EmailNode *temp = curr->nexte;
                curr->nexte = curr->nexte->nexte;
                temp->nexte = nullptr;
                delete temp;
                return true;
            }
            curr = curr->nexte;
        }
    }
    if ( !firsts ) {
        return false;
    } else {
        GroupNode *prevg = firsts;
        GroupNode *currg = prevg->nexts;
        while ( !prevg->sub->removeAddress( name )) {
            if ( !currg ) {
                return false;
            } else {
                prevg = currg;
                currg = currg->nexts;
            }
        }
        return true;
    }
    return false;
}


GroupNode * Group::findGroup( const std::string &name ) {
    if ( !firsts ) {
        return nullptr;
    } else {
        GroupNode *prev = firsts;
        GroupNode *curr = prev->nexts;
        if ( prev->sub->name() == name ) {
            return prev;
        } else {
            while ( !prev->sub->removeGroup( name )) {
                if ( !curr ) {
                    return nullptr;
                }
                if ( curr->sub->name() == name ) {
                    return curr;
                } else {
                    prev = curr;
                    curr = curr->nexts;
                }
            }
        }
    }
    return nullptr;
}


EmailNode * Group::findAddress( const std::string & name ) {
    EmailNode *curr = firste;
    if ( curr ) {
        if ( curr->address == name ) {
            return curr;
        }
        while ( curr->nexte ) {
            if ( curr->nexte->address == name ) {
                return curr->nexte;
            }
            curr = curr->nexte;
        }
    }
    if ( !firsts ) {
        return nullptr;
    } else {
        GroupNode *prevg = firsts;
        GroupNode *currg = prevg->nexts;
        EmailNode *temp = nullptr;
        while ( !temp ) {
            if ( !currg ) {
                return nullptr;
            } else {
                prevg = currg;
                currg = currg->nexts;
            }
            temp = prevg->sub->findAddress( name );
        }
        return temp;
    }
    return nullptr;
}


void printGroup( std::ostream &out, int n , Group & group ) {
    for ( int i = 0; i < n; ++i ){
        out << "\t";
    }
    out << group.name() << std::endl;
    for ( int i = 0; i < n; ++i ){
        out << "\t";
    }
    out<<group.emails()<<std::endl;
    GroupNode *temp = group.firsts;
    while( temp ){
        printGroup( out, n+1 , *temp->sub );
        temp = temp->nexts;
    }
}

std::ostream & operator<<( std::ostream &out, Group & group ) {
    printGroup(out, 0, group);
    return out;
}

std::ostream & operator<<( std::ostream &out, GroupNode & groupnode ) {
    out << *groupnode.sub;
    return out;
}

std::ostream & operator<<( std::ostream &out, EmailNode & emailnode ) {
    out << emailnode.address;
    return out;
}
