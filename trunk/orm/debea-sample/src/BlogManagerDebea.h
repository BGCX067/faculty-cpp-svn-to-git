/*
 * BlogManager.h
 *
 *      Author: Hasan
 */

#ifndef BLOGMANAGERDEBEA_H_
#define BLOGMANAGERDEBEA_H_

#include "dba/dba.h"
#include "dba/double_filter.h"
#include "dba/stdvector.h"

#include <vector>
#include <list>
#include <string>
#include <iostream>
using namespace std;

class Post;

class Tag : public dba::Storeable {
	DECLARE_STORE_TABLE();
public:
	string name;
	list< Post > posts;
};

class Comment : public dba::Storeable {
	DECLARE_STORE_TABLE();
public:
	double identifier;
	// TODO check datetime support
	std::string text;
};

BEGIN_STORE_TABLE(Comment, dba::Storeable, "comment_table")
  BIND_STR(Comment::identifier, dba::Double, "col_identifier")
  BIND_STR(Comment::text, dba::String, "col_text")
END_STORE_TABLE()

class Post : public dba::Storeable {
	DECLARE_STORE_TABLE();
public:
	std::string title;
	std::string text;
	vector< Comment > comments;
	list< Tag > tags;

	bool operator==(const Post& post) {
		return title == post.title;
	}
};

BEGIN_STORE_TABLE(Tag, dba::Storeable, "tag_table")
  BIND_STR(Tag::name, dba::String, "col_name")
  //BIND_CLA(Tag::posts, dba::stdList< Post >, "fk_tag", NULL, "post_tag_table")
END_STORE_TABLE()

//Class name, parent class name and relation name
//(SQL table in case of SQL database)
BEGIN_STORE_TABLE(Post, dba::Storeable, "post_table")
  BIND_STR(Post::title, dba::String, "col_title")
  BIND_STR(Post::text, dba::String, "col_text")
  BIND_COL(Post::comments, dba::stdVector< Comment >, "fk_post")
  //BIND_CLA(Post::tags, dba::stdList< Tag >, "fk_post", NULL, "post_tag_table")
END_STORE_TABLE()

class Person : public dba::Storeable {
	DECLARE_STORE_TABLE();
public:
	int userId;
};

class User : public Person {
	DECLARE_STORE_TABLE();
public:
	std::string name;
	std::string surname;
	std::list<Post> posts;
};

BEGIN_STORE_TABLE(Person, dba::Storeable, "person_table")
  //we store mStrVal in varchar database field named
  //"strval" using dba::String as conversion filter
  BIND_INT(Person::userId, dba::Int, "col_id")
END_STORE_TABLE()

//Class name, parent class name and relation name
//(SQL table in case of SQL database)
BEGIN_STORE_TABLE(User, Person, "user_table")
  //we store mStrVal in varchar database field named
  //"strval" using dba::String as conversion filter
  BIND_STR(User::name, dba::String, "col_name")
  BIND_STR(User::surname, dba::String, "col_surname")
  BIND_COL(User::posts, dba::stdList< Post >, "fk_user")
END_STORE_TABLE()

//begin of SQL schema
dba::SQL counter_create(
"CREATE TABLE debea_object_count ("
"  id INT"
")");

dba::SQL post_create(
"CREATE TABLE post_table ("
"  id INT PRIMARY KEY,"
"  col_title VARCHAR(10),"
"  col_text VARCHAR(20),"
"  fk_user INT NOT NULL"
")");

dba::SQL person_create(
"CREATE TABLE person_table ("
"  id INT PRIMARY KEY,"
"  col_id INT"
")");

dba::SQL user_create(
"CREATE TABLE user_table ("
"  id INT PRIMARY KEY,"
"  col_name VARCHAR(20),"
"  col_surname VARCHAR(20)"
")");

dba::SQL comment_create(
"CREATE TABLE comment_table ("
"  id INT PRIMARY KEY,"
"  col_identifier INT,"
"  col_text VARCHAR(20),"
"  fk_post INT NOT NULL"
")");

dba::SQL tag_create(
"CREATE TABLE tag_table ("
"  id INT PRIMARY KEY,"
"  col_name VARCHAR(10)"
")");

//note that we need to add special
//dba_coll_id column for BIND_CLA
dba::SQL post_tag_create(
"CREATE TABLE post_tag_table ("
"  id INT PRIMARY KEY,"
"  fk_post INT NOT NULL,"
"  fk_tag INT NOT NULL,"
"  dba_coll_id INT NOT NULL"
")");

//end of SQL schema

class BlogManagerDebea {
public:
	BlogManagerDebea();
	virtual ~BlogManagerDebea();

	void prepareDB(dba::SQLArchive& pAr);
	void createUsers(dba::SQLArchive& pAr);
	void printDB(dba::SQLArchive& pAr);
	void printUserInfo(User & u);
};

#endif /* BLOGMANAGERDEBEA_H_ */
