protoc ./login.proto --cpp_out=../server/genproto
protoc ./map.proto --cpp_out=../server/genproto
cd ../server/genproto
