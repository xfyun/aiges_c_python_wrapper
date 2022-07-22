#include "pyWrapper.h"

//namespace py = pybind11;
//using namespace std::chrono_literals;


const char* WrapperFile = "main";
const char* WrapperClass = "Wrapper";


PYBIND11_EMBEDDED_MODULE(aiges_embed, module){

  py::class_<ResponseData> responseData(module, "ResponseData");
  responseData.def(py::init<>())
	  .def_readwrite("key",&ResponseData::key)
	  .def_readwrite("value", &ResponseData::value)
	  .def_readwrite("status", &ResponseData::status)
	  .def_readwrite("len", &ResponseData::len);
  py::class_<Response> response(module, "Response");
  response.def(py::init<>())
	  .def_readwrite("list",&Response::list);
}



PyWrapper::PyWrapper()
  {
    py::gil_scoped_acquire acquire;
    _obj = py::module::import(WrapperFile).attr(WrapperClass)();

    _wrapperInit = _obj.attr("wrapperInit");
    _wrapperFini = _obj.attr("wrapperFini");
    _wrapperOnceExec = _obj.attr("wrapperOnceExec");
    _wrapperError = _obj.attr("wrapperError");
    _wrapperTest = _obj.attr("wrapperTestFunc");

  }

PyWrapper::~PyWrapper()
  {
    _wrapperError.release();
    _wrapperInit.release();
    _wrapperFini.release();
    _wrapperOnceExec.release();
    _wrapperTest.release();
  }


int PyWrapper::wrapperInit(std::map<std::string, std::string> config)
{
    py::gil_scoped_acquire acquire;
    return _wrapperInit(config).cast<int>();
}


int PyWrapper::wrapperFini()
{
    py::gil_scoped_acquire acquire;
    return _wrapperFini().cast<int>();
}

int PyWrapper::wrapperOnceExec(std::map<std::string,std::string> params, std::vector<py::object> reqData, std::vector<py::object> respData, std::string sid) {

    py::gil_scoped_acquire acquire;
    try {
        py::object r =  _wrapperOnceExec(params, reqData);
    } catch (py::error_already_set &e) {
	std::cout <<"errC"<<std::endl;
	return -1;
    }
    
    return 0;

}


std::string PyWrapper::wrapperError(int err){
     py::gil_scoped_acquire acquire;
    return _wrapperError(err).cast<std::string>();
}


int PyWrapper::wrapperTest() {
   py::gil_scoped_acquire acquire;
        std::vector<py::dict> req;
	py::dict item;
	item["key"] = "33"; 
	item["data"] = "ccccc";
	item["len"] = 11;
	item["type"] =22;
	req.push_back(item);
   std::vector<py::object>  resp;
   resp.push_back(py::str("cfc"));
   py::object  ret =  _wrapperTest(req, resp);
   Response *l;
    try {
         l = ret.cast<Response *>(); 
       // py::object r =  _wrapperOnceExec(params, reqData);
    } catch (py::cast_error &e) {
	std::cout <<"errC"<<std::endl;
	return -1;
    }
    return 0;
  for (int i = 0; i<l->list.size(); ++i)
	{
		ResponseData d = l->list[i];
  std::cout <<"Response key: " <<d.key << std::endl;
  std::cout <<"Response len" <<d.len <<std::endl;
 
	}
//  auto message = ret.cast<std::vector<std::string>>();
 // printf("%s,:", message[0].c_str());
 // printf("%s,:", message[1].c_str());
 //  printf("resp len%d\n",resp.size());
//   printf("%d", ret);


}
