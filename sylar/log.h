#ifndef __SYLAR_LOG_H
#define __SYLAR_LOG_H

#include <string>
#include <sstream>
#include <memory>

namespace sylar{

class Logger;
class LoggerManager;

//日志级别
class LogLevel{
public:
  /**
   * @brief 日志级别枚举
   */
  enum Level{
     //未知级别
     UNKNOW = 0,
     //Debug 级别
     DEBUG = 1,
     //Info 级别
     INFO = 2,
     //WARN 级别
     WARN = 3,
     //ERROR 级别
     ERROE = 4,
     //FATAL 级别
     FATAL = 5
  };

  /**
   * @brief 将日志级别转成文本输出
   * @param[in] level 日志级别
   */ 
  static const char* ToString(LogLevel::Level level);

  /**
   * @brief 将文本转换为日志级别
   * @para[in] str 日志级别文本
   */ 
  static LogLevel::Level FromString(const std::string& str);
};

//日志事件
class LogEvent{
public:
  LogEvent();
private:
  //文件名
  const char* m_file = nullptr;
  //行号
  int32_t m_line = 0;
  //程序启动到现在的毫秒数
  uint32_t m_elapse = 0;
  //线程ID
  uint32_t m_threadID = 0;
  //协程ID
  uint32_t m_fiberID = 0;
  //时间戳
  uint64_t m_time = 0;
  //线程名称
  std::string m_threadName;
  //日志内容流
  std::stringstream m_ss;
  //日志器
  std::shared_ptr<Logger> m_logger;
  //日志等级
  LogLevel::Level m_level;
};

//日志器
class Logger{
public:
  Logger();
private:
};

//日志输出地
class LogAppender{
public:
  // @brief 析构函数
  virtual ~LogAppender(){}
  //
private:
};

//日志输出格式
class LogFormat{
public:

private:
};

}


#endif
