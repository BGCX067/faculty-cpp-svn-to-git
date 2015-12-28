/*
 * BlogManagerDebea.cpp
 *
 * Testing some Debea library features
 *
 *      Author: Hasan
 */

#include "BlogManagerDebea.h"

BlogManagerDebea::BlogManagerDebea() {
}

BlogManagerDebea::~BlogManagerDebea() {
}

void BlogManagerDebea::createUsers(dba::SQLArchive& pAr) {
	dba::SQLOStream ostream = pAr.getOStream();
	ostream.open();

	// create some tags to be used in M-N test
	for( int n=0; n<3; n++) {
		Tag t;
		char cc[10];
		sprintf(cc,"tag-%d", n);
		t.name = cc;
		ostream.put(&t); //id of object is assigned here
		std::cout << "Tag '" << t.name << "' is stored with id = " << t.getId() << std::endl;
	}


	//construct some user instances with some posts
	for (int i = 0; i < 5; i++) {
		User u;
		u.userId = i + 100;

		char ss[10];
		sprintf(ss, "user-%d", i);
		u.name = ss;

		char uu[10];
		sprintf(uu, "surname-%d", i);
		u.surname = uu;

		for (int j = 0; j < 3; j++) {
			Post p;
			char ii[10];
			sprintf(ii, "title-%d", j);
			p.title = ii;

			char tt[10];
			sprintf(tt, "text-%d", j);
			p.text = tt;

			for( int k=0; k<2; k++) {
				Comment c;
				char cc[10];
				sprintf(cc,"commenttext-%d", k);
				c.text = cc;
				c.identifier = 1234.567 * (k+1);

				p.comments.push_back(c);
			}

			Tag t;
			dba::SQLIStream istream = pAr.getIStream();
			istream.setWhereId(j);
			istream.get(&t);
			istream.close();
			p.tags.push_back( t );

			u.posts.push_back(p);
		}

		//this should generate 16 SQL inserts (1 Foo + 10 Bars + 5 CuBars)
		ostream.put(&u); //id of object is assigned here
		std::cout << "User is stored with id = " << u.getId() << std::endl;
	}

	//destroy stream and release connection
	ostream.destroy();
}

void BlogManagerDebea::printUserInfo(User & u) {

	cout << "User [id:" << u.userId << ", name:" << u.name << ", surname:" << u.surname << std::endl << "\t, posts {";
	for( list<Post>::iterator it = u.posts.begin(); it != u.posts.end(); it++) {
		cout << "\n\t( title:" << (*it).title << ", text:" << (*it).text;
		cout << "\n\t\t, comments {";
		for( vector<Comment>::iterator c = (*it).comments.begin(); c != (*it).comments.end(); c++) {
			cout << "\n\t\t( identifier:" << (*c).identifier << ", text:" << (*c).text << " )";
		}
		cout << "\n\t\t}" << std::endl;

		cout << "\n\t\t, tags {";
		for( list<Tag>::iterator t = (*it).tags.begin(); t != (*it).tags.end(); t++) {
			cout << (*t).name << ", ";
		}
		cout << "}" << std::endl;
	}

	cout << "\t}";
	cout << "]" << std::endl;
}

void BlogManagerDebea::printDB(dba::SQLArchive& pAr) {
	User u;
	int res;
	do {

		cout << "enter user id (-1 to end, 0 for tags, 99 for query by post title, 98 for query User by name): ";
		cin >> res;
		if (res == 0) {
			Tag t;

			dba::SQLIStream istream = pAr.getIStream();
			istream.open(t);
			while(istream.getNext(&t)) {
				cout << "tag name: " << t.name << ", postcount: " << t.posts.size() << endl;
			}
		} else if (res == 99) {
			std::string title, text;
			cout << std::endl << "enter post title: ";
			cin >> title;

			dba::SQLIStream istream = pAr.getIStream();
			std::auto_ptr<dba::DbResult> res(istream.sendQuery(dba::SQL("select col_text from post_table where col_title=':s'").into(text) << title));
			if (res->fetchRow()) {
				cout << std::endl << "text of requested title is '" << text << "'" << std::endl;
			}
		} else if (res == 98) {
			std::string name, surname;
			cout << std::endl << "enter user name: ";
			cin >> name;

			dba::SQLIStream istream = pAr.getIStream();
			istream.setWherePart(dba::SQL("col_name=':s'") << name);
			if (istream.get(&u)) {
				istream.close();
				printUserInfo(u);
			} else {
				cout << "no user exists with the given name!" << std::endl;
			}
		} else if (res > 0) {
			//Now load everything back
			dba::SQLIStream istream = pAr.getIStream();

			//tell stream which User object to load. Posts will
			//be retrieved using foreign key fields automatically.
			istream.setWhereId(res);

			if (istream.get(&u)) {
				istream.close();

				printUserInfo(u);
			} else {
				cout << "no user exists with the given id." << std::endl;
			}
		}
	} while (res != -1);

	cout << "bye...";

}

void BlogManagerDebea::prepareDB(dba::SQLArchive& ar) {
	//set object responsible for assigning id numbers
	//for serialized objects.
	//We use generic one that needs special SQL table
	//debea_object_count.
	ar.setIdFetcher(new dba::GenericFetcher());

	//sqlite3 backend creates new empty database
	//if there is no file
	unlink("blogdb.sqt3");

	//this will load database plugin and create new empty database
	ar.open("dbasqlite3", "dbname=blogdb.sqt3");

	//create needed tables
	ar.getOStream().sendUpdate(counter_create);
	ar.getOStream().sendUpdate(post_create);
	ar.getOStream().sendUpdate(comment_create);
	ar.getOStream().sendUpdate(tag_create);
	ar.getOStream().sendUpdate(person_create);
	ar.getOStream().sendUpdate(user_create);
	ar.getOStream().sendUpdate(post_tag_create);


	//first id
	ar.getOStream().sendUpdate(dba::SQL(
			"INSERT INTO debea_object_count VALUES (:d)") << 1);
}

int main() {
	try {
		dba::SQLArchive ar;
		BlogManagerDebea blogManager;

		blogManager.prepareDB(ar);
		blogManager.createUsers(ar);
		blogManager.printDB(ar);

		return 0;
	} catch (const dba::SQLException& pEx) {
		std::cout << "SQL Error: " << pEx.what() << std::endl;
		std::cout << "While executing: " << std::endl << pEx.getQuery()
				<< std::endl;
	} catch (const dba::Exception& pEx) {
		std::cout << "Error: " << pEx.what() << std::endl;
		return -1;
	};
}
