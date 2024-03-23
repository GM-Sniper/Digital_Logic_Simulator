 // else if (it->first == "NOR3")
            // {
            //     cout << "NOR3 Gate " << endl;
            //     cout << "Inputs : \n"
            //          << "First : " << it->second[1].name << " Boolean state : " << getWire(ioComponents, it->second[1].name) << endl;
            //     cout << "Second : " << it->second[2].name << " Boolean state : " << getWire(ioComponents, it->second[2].name) << endl;
            //     cout << "Third : " << it->second[3].name << " Boolean state : " << getWire(ioComponents, it->second[3].name) << endl;
            //     it->second[0].type = !(getWire(ioComponents, it->second[1].name) | (getWire(ioComponents, it->second[2].name) | getWire(ioComponents, it->second[3].name)));
            //     cout << "Output : " << it->second[0].name << "  Boolean state : " << getWire(ioComponents, it->second[0].name) << endl;
            //     if (it->second[0].initial.empty())
            //     {
            //         it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
            //     }
            //     else if (it->second[0].initial.top() != it->second[0].type)
            //     {
            //         it->second[0].delay = maxOfThree(getDelay(ioComponents, it->second[1].name), getDelay(ioComponents, it->second[2].name), getDelay(ioComponents, it->second[3].name)) + libComponents[position].getDelayTime();
            //         // outfile << getDelay(ioComponents, it->second[0].name) << " " << it->second[0].name << " " << it->second[0].type << endl;
            //         F_output.push_back({getDelay(ioComponents, it->second[0].name), it->second[0].name, it->second[0].type});

            //         it->second[0].initial.push(getWire(ioComponents, it->second[0].name));
            //     }

            //     cout << "===============================" << endl;
            // }
