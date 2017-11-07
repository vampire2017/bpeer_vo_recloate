/**
 * Autogenerated by Thrift Compiler (0.10.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef WithReturnService_H
#define WithReturnService_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include "withReturn_types.h"



#ifdef _WIN32
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

class WithReturnServiceIf {
 public:
  virtual ~WithReturnServiceIf() {}
  virtual void resultReturn(std::string& _return, const std::string& img_, const std::string& ID_, const Data& data) = 0;
};

class WithReturnServiceIfFactory {
 public:
  typedef WithReturnServiceIf Handler;

  virtual ~WithReturnServiceIfFactory() {}

  virtual WithReturnServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(WithReturnServiceIf* /* handler */) = 0;
};

class WithReturnServiceIfSingletonFactory : virtual public WithReturnServiceIfFactory {
 public:
  WithReturnServiceIfSingletonFactory(const boost::shared_ptr<WithReturnServiceIf>& iface) : iface_(iface) {}
  virtual ~WithReturnServiceIfSingletonFactory() {}

  virtual WithReturnServiceIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(WithReturnServiceIf* /* handler */) {}

 protected:
  boost::shared_ptr<WithReturnServiceIf> iface_;
};

class WithReturnServiceNull : virtual public WithReturnServiceIf {
 public:
  virtual ~WithReturnServiceNull() {}
  void resultReturn(std::string& /* _return */, const std::string& /* img_ */, const std::string& /* ID_ */, const Data& /* data */) {
    return;
  }
};

typedef struct _WithReturnService_resultReturn_args__isset {
  _WithReturnService_resultReturn_args__isset() : img_(false), ID_(false), data(false) {}
  bool img_ :1;
  bool ID_ :1;
  bool data :1;
} _WithReturnService_resultReturn_args__isset;

class WithReturnService_resultReturn_args {
 public:

  WithReturnService_resultReturn_args(const WithReturnService_resultReturn_args&);
  WithReturnService_resultReturn_args& operator=(const WithReturnService_resultReturn_args&);
  WithReturnService_resultReturn_args() : img_(), ID_() {
  }

  virtual ~WithReturnService_resultReturn_args() throw();
  std::string img_;
  std::string ID_;
  Data data;

  _WithReturnService_resultReturn_args__isset __isset;

  void __set_img_(const std::string& val);

  void __set_ID_(const std::string& val);

  void __set_data(const Data& val);

  bool operator == (const WithReturnService_resultReturn_args & rhs) const
  {
    if (!(img_ == rhs.img_))
      return false;
    if (!(ID_ == rhs.ID_))
      return false;
    if (!(data == rhs.data))
      return false;
    return true;
  }
  bool operator != (const WithReturnService_resultReturn_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const WithReturnService_resultReturn_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class WithReturnService_resultReturn_pargs {
 public:


  virtual ~WithReturnService_resultReturn_pargs() throw();
  const std::string* img_;
  const std::string* ID_;
  const Data* data;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _WithReturnService_resultReturn_result__isset {
  _WithReturnService_resultReturn_result__isset() : success(false) {}
  bool success :1;
} _WithReturnService_resultReturn_result__isset;

class WithReturnService_resultReturn_result {
 public:

  WithReturnService_resultReturn_result(const WithReturnService_resultReturn_result&);
  WithReturnService_resultReturn_result& operator=(const WithReturnService_resultReturn_result&);
  WithReturnService_resultReturn_result() : success() {
  }

  virtual ~WithReturnService_resultReturn_result() throw();
  std::string success;

  _WithReturnService_resultReturn_result__isset __isset;

  void __set_success(const std::string& val);

  bool operator == (const WithReturnService_resultReturn_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const WithReturnService_resultReturn_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const WithReturnService_resultReturn_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _WithReturnService_resultReturn_presult__isset {
  _WithReturnService_resultReturn_presult__isset() : success(false) {}
  bool success :1;
} _WithReturnService_resultReturn_presult__isset;

class WithReturnService_resultReturn_presult {
 public:


  virtual ~WithReturnService_resultReturn_presult() throw();
  std::string* success;

  _WithReturnService_resultReturn_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

class WithReturnServiceClient : virtual public WithReturnServiceIf {
 public:
  WithReturnServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  WithReturnServiceClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void resultReturn(std::string& _return, const std::string& img_, const std::string& ID_, const Data& data);
  void send_resultReturn(const std::string& img_, const std::string& ID_, const Data& data);
  void recv_resultReturn(std::string& _return);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class WithReturnServiceProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  boost::shared_ptr<WithReturnServiceIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (WithReturnServiceProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_resultReturn(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  WithReturnServiceProcessor(boost::shared_ptr<WithReturnServiceIf> iface) :
    iface_(iface) {
    processMap_["resultReturn"] = &WithReturnServiceProcessor::process_resultReturn;
  }

  virtual ~WithReturnServiceProcessor() {}
};

class WithReturnServiceProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  WithReturnServiceProcessorFactory(const ::boost::shared_ptr< WithReturnServiceIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::boost::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::boost::shared_ptr< WithReturnServiceIfFactory > handlerFactory_;
};

class WithReturnServiceMultiface : virtual public WithReturnServiceIf {
 public:
  WithReturnServiceMultiface(std::vector<boost::shared_ptr<WithReturnServiceIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~WithReturnServiceMultiface() {}
 protected:
  std::vector<boost::shared_ptr<WithReturnServiceIf> > ifaces_;
  WithReturnServiceMultiface() {}
  void add(boost::shared_ptr<WithReturnServiceIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  void resultReturn(std::string& _return, const std::string& img_, const std::string& ID_, const Data& data) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->resultReturn(_return, img_, ID_, data);
    }
    ifaces_[i]->resultReturn(_return, img_, ID_, data);
    return;
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class WithReturnServiceConcurrentClient : virtual public WithReturnServiceIf {
 public:
  WithReturnServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  WithReturnServiceConcurrentClient(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(boost::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, boost::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  void resultReturn(std::string& _return, const std::string& img_, const std::string& ID_, const Data& data);
  int32_t send_resultReturn(const std::string& img_, const std::string& ID_, const Data& data);
  void recv_resultReturn(std::string& _return, const int32_t seqid);
 protected:
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  boost::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  ::apache::thrift::async::TConcurrentClientSyncInfo sync_;
};

#ifdef _WIN32
  #pragma warning( pop )
#endif



#endif
