#include <core/server.h>

int main(int argc, char* argv[])
{
    return Server(8080, "local").run();

    //ClientMetadata client1{{}, "1123423423", "dvsdafgasdfsdfsda fsdfsdafdsfsda fsdfsdf"};
    //ClientMetadata client2{{}, "123", " fsdfsdafdsfsda fsdfsdf dvsdafgasdfsdfsda fsdfsdafdsfsda fsdfsdf"};

    //std::vector<ClientMetadata> v;
    //v.push_back(client1);
    //v.push_back(client2);

    //std::vector<unsigned char> serialized_v = Network::Serializer::serialize(v);
    //std::vector<ClientMetadata> deserialized_v = Network::Serializer::deserialize<std::vector<ClientMetadata>>(
    //    serialized_v);

    //std::vector<unsigned char> serialized_client1 = Network::Serializer::serialize(client1);
    //ClientMetadata deserialized_client1 = Network::Serializer::deserialize<ClientMetadata>(serialized_client1);
}
