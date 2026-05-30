#include "compiler.h"
#include "utils.h"

#include <iostream>



std::string SQL_compiler(SQL& db, std::string str){
  int pos;
  while(str.find(';') != std::string::npos){
    pos = str.find(';');
    std::string line = str.substr(0, pos);
    /////////////////////////////////////////
    int space_index = line.find(' ');
    std::string opp = line.substr(0,space_index);
    if(opp == "CREATE"){
      
      line = line.substr( space_index + 1, line.length() - 1);
      space_index = line.find(' ');
      opp = line.substr(0,space_index);
      
      if(opp == "TABLE"){
        line = line.substr(space_index + 1, line.length() - 1);
        space_index = line.find(' ');
        std::string db_name;
        if(space_index != std::string::npos){
            db_name = line.substr(0,space_index);
        }else{
            std::cout << "Unknown command" << std::endl; 
        }
        line = line.substr( space_index + 1, line.length() - 1);
        int left = line.find('(');
        int right = line.find(')');
        opp = line.substr(left , right);
        std::vector<std::string> key_vec;
        int key_pos;
        std::string key_value;
        while(opp.find(',') != std::string::npos){
            key_pos = opp.find(',');
            key_value = opp.substr(1, key_pos - 1);
            ////////////////////////////////
            key_vec.push_back(key_value);            
            ////////////////////////////////
            opp.erase(1, key_pos + 1);
        }
        key_pos = opp.find(')');
        key_value = opp.substr(1, key_pos - 1);
        key_vec.push_back(key_value);       
        
        db.create_table(key_vec, db_name);
        return db_name + " created \n";
      }
    }else if(opp == "INSERT"){
      std::cout << opp << std::endl ;
      line = line.substr( space_index + 1, line.length() - 1);
      space_index = line.find(' ');
      opp = line.substr(0,space_index);
      
      if(opp == "INTO"){

        line = line.substr( space_index + 1, line.length() - 1);
        space_index = line.find(' ');
        std::string db_name = line.substr(0,space_index);

        line = line.substr( space_index + 1, line.length() - 1);
        space_index = line.find(' ');
        opp = line.substr(0,space_index);
        if(opp == "VALUES"){
            line = line.substr( space_index + 1, line.length() - 1);
            int left = line.find('(');
            int right = line.find(')');
            opp = line.substr(left , right);
            std::vector<std::string> key_vec;
            int key_pos;
            std::string key_value;
            while (opp.find(',') != std::string::npos) {
                key_pos = opp.find(',');
                key_value = opp.substr(1, key_pos - 1);
                key_vec.push_back(clean(key_value));
                opp.erase(1, key_pos + 1);
            }
            key_pos = opp.find(')');
            key_value = opp.substr(1, key_pos - 1);
            key_vec.push_back(clean(key_value));
            db(db_name)->insert(db_name, key_vec);
            return "row inserted into " + db_name + "\n";
        }
      }
    }else if(opp == "SELECT"){
      
        line = line.substr( space_index + 1, line.length() - 1);
        space_index = line.find(' ');
        opp = line.substr(0,space_index);
        bool take_all = false;

        if(opp == "*"){
                line = line.substr( space_index + 1, line.length() - 1);
                space_index = line.find(' ');
                opp = line.substr(0,space_index);
            if(opp == "FROM"){
                line = line.substr( space_index + 1, line.length() - 1);
                space_index = line.find(' ');
                std::string db_name = line.substr(0,space_index);
                if(space_index == std::string::npos){
                    return db(db_name)->to_string();
                }else{
                    line = line.substr( space_index + 1, line.length() - 1);
                    space_index = line.find(' ');
                    opp = line.substr(0,space_index);
                    std::string key_value;
                    std::string name_value;
                    if(opp == "WHERE"){
                        line = line.substr( space_index + 1, line.length() - 1);
                        space_index = line.find(' ');
                        key_value = line.substr(0,space_index);

                        line = line.substr( space_index + 1, line.length() - 1);
                        space_index = line.find(' ');
                        opp = line.substr(0,space_index);
                        if(opp == "="){
                            line = line.substr( space_index + 1, line.length() - 1);
                            space_index = line.find(' ');
                            name_value = line.substr(0,space_index);
                            Table result =db(db_name)->select(key_value, name_value);
                            return result.to_string();
                        }
                    }
                }
            }
        }
    }else if(opp == "DELETE"){
      
        line = line.substr( space_index + 1, line.length() - 1);
        space_index = line.find(' ');
        opp = line.substr(0,space_index);
        bool take_all = false;

        if(opp == "*"){
                line = line.substr( space_index + 1, line.length() - 1);
                space_index = line.find(' ');
                opp = line.substr(0,space_index);
            if(opp == "FROM"){
                line = line.substr( space_index + 1, line.length() - 1);
                space_index = line.find(' ');
                std::string db_name = line.substr(0,space_index);
                if(space_index == std::string::npos){
                    db(db_name)->delete_from();
                }else{
                    line = line.substr( space_index + 1, line.length() - 1);
                    space_index = line.find(' ');
                    opp = line.substr(0,space_index);
                    std::string key_value;
                    std::string name_value;
                    if(opp == "WHERE"){
                        line = line.substr( space_index + 1, line.length() - 1);
                        space_index = line.find(' ');
                        key_value = line.substr(0,space_index);

                        line = line.substr( space_index + 1, line.length() - 1);
                        space_index = line.find(' ');
                        opp = line.substr(0,space_index);
                        if(opp == "="){
                            line = line.substr( space_index + 1, line.length() - 1);
                            space_index = line.find(' ');
                            name_value = line.substr(0,space_index);
                            db(db_name)->delete_from(key_value, name_value);
                            return db_name + " deleted \n";
                        }
                    }
                }
            }
        }
    }else if(opp == "UPDATE"){
    std::string key_value;
    std::string name_value;
    std::string new_key_value;
    std::string new_name_value;

    line = line.substr(space_index + 1);
    space_index = line.find(' ');
    std::string db_name = line.substr(0, space_index);

    line = line.substr(space_index + 1);
    space_index = line.find(' ');
    opp = line.substr(0, space_index);

    if(opp == "SET"){
        line = line.substr(space_index + 1);
        space_index = line.find(' ');
        key_value = line.substr(0, space_index);

        line = line.substr(space_index + 1);
        space_index = line.find(' ');
        opp = line.substr(0, space_index);

        if(opp == "="){
            line = line.substr(space_index + 1);
            space_index = line.find(' ');
            name_value = line.substr(0, space_index);

            line = line.substr(space_index + 1);
            space_index = line.find(' ');
            opp = line.substr(0, space_index);

            if(opp == "WHERE"){
                line = line.substr(space_index + 1);
                space_index = line.find(' ');
                new_key_value = line.substr(0, space_index);

                line = line.substr(space_index + 1);
                space_index = line.find(' ');
                opp = line.substr(0, space_index);

                if(opp == "="){
                    line = line.substr(space_index + 1);
                    new_name_value = line;

                    db(db_name)->update_set(new_key_value, clean(new_name_value), key_value, clean(name_value));
                    return db_name + " updated \n";
                }
            }
        }
    }
}
    //////////////////////////////////////////
    str.erase(0, pos + 1);
  }

  return "";
}