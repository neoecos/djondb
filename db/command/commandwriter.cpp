#include "commandwriter.h"
#include "insertcommand.h"
#include "findbykeycommand.h"
#include "bsonoutputstream.h"
#include <memory>
#include <iostream>

using namespace std;

CommandWriter::CommandWriter(OutputStream* out)
{
    _stream = out;
}

CommandWriter::~CommandWriter()
{
    //dtor
}


CommandWriter::CommandWriter(const CommandWriter& orig) {
    this->_stream = orig._stream;
}

int writeInsert(InsertCommand* cmd, OutputStream* out)  {
    const std::string* ns = cmd->nameSpace();
    out->writeString(ns);

    std::auto_ptr<BSONOutputStream> bsonout(new BSONOutputStream(out));
    bsonout->writeBSON(*cmd->bson());

    return 0;
}

int writeFindByKey(FindByKeyCommand* cmd, OutputStream* out)  {
    const std::string* ns = cmd->nameSpace();
    out->writeString(ns);

    std::auto_ptr<BSONOutputStream> bsonout(new BSONOutputStream(out));
    bsonout->writeBSON(*cmd->bson());

    return 0;
}

int CommandWriter::writeCommand(Command* cmd) {
    int type = cmd->commandType();
    _stream->writeInt(type);

    int ret = 0;
    switch (type) {
        case INSERT: // Insert
            ret = writeInsert((InsertCommand*)cmd, _stream);
            break;
        case FINDBYKEY:
            ret = writeFindByKey((FindByKeyCommand*)cmd, _stream);
            break;
        case CLOSECONNECTION: // Nothing to be done
            break;
        default:
            cout << "Unknown type: " << type << endl;
            ret = 1;
            break;
    }
    return ret;
}
