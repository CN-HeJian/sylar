#ifndef __SYLAR_LOG_H
#define __SYLAR_LOG_H

#include <string>
#include <sstream>
#include <memory>
#include <list>

namespace sylar{

class Logger;
class LoggerManager;
class LogAppender;

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
  //外部调用指针
  typedef std::shared_ptr<LogEvent> ptr;
  /**
   * @ breif 构造函数
   *  @param[in]  logger        日志器
   *  @param[in]  levle         级别
   *  @param[in]  file          文件名
   *  @param[in]  line          文件行号
   *  @param[in]  elapse        程序启动到现在的毫秒数
   *  @param[in]  thread_id     线程id
   *  @param[in]  fiber_id      协程id
   *  @param[in]  time          日志时间
   *  @param[in]  thread_name   线程名称
   */
  LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level
          ,const char* file, int32_t line, uint32_t elapse
          ,uint32_t thread_id, uint32_t fiber_id, uint64_t time
          ,const std::string& thread_name); 

  /**
   * @brief 返回文件名
   */ 
  const char* getFile() const {  
    return m_file;
  }

  /**
   * @brief 返回行号
   */ 
  int32_t getLine() const { 
    return m_line;
  }

  /**
   * @brief 返回耗时
   */
  uint32_t getElapse() const{
    return m_elapse;
  }
  
  /**
   * @brief 返回线程ID
   */
  uint32_t getThreadId() const{
    return m_threadId;
  }
   
  /**
   * @brief 返回协程ID
   */
  uint32_t getFiberId() const{
    return m_fiberId;
  }

  /**
   * @brief 返回时间
   */
   uint64_t getTime() const{
     return m_time;
   }
  
  /**
   * @brief 返回线程名称
   */
  const std::string& getThreadName() const{
     return m_threadName;
  }
  
  /**
   * @brief 返回日志内容
   */
  std::string getContent() const{
     return m_ss.str();
  } 

  /**
   * @brief 返回日志器
   */
  std::shared_ptr<Logger> getLogger() const{
    return m_logger;
  }

  /**
   * @brief 返回日志级别
   */
  LogLevel::Level getLevel() const{
    return m_level;
  }

  /**
   * @brief 返回日志内容字符串流
   */
  std::stringstream& getSS(){
    return m_ss;
  }

  /**
   * @breif 格式化写入日志内容
   */
  void format(const char* fmt,...);

  /**
   * @brief 格式化写入日志内容
   */
  void format(const char* fmt,va_list al);

private:
  //文件名
  const char* m_file = nullptr;
  //行号
  int32_t m_line = 0;
  //程序启动到现在的毫秒数
  uint32_t m_elapse = 0;
  //线程ID
  uint32_t m_threadId = 0;
  //协程ID
  uint32_t m_fiberId = 0;
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

/**
 * @brief 日志输出地
 */
class LogAppender{
public:
  typedef std::shared_ptr<LogAppender> ptr;
  // @brief 析构函数
  virtual ~LogAppender(){}
  //
private:
};

/**
 * @brief 日志器 
 */
class Logger : public std::enable_shared_from_this<Logger>{
public:
  typedef std::shared_ptr<Logger> ptr;
   
  /**
   * @brief 构造函数
   * @param[in] name 日志器名称
   */ 
  Logger(const std::string& name = "root");
  
  /**
   * @brief 写日志
   * @param[in] level 日志级别
   * @param[in] event 日志事件
   */ 
  void log(LogLevel::Level level, LogEvent::ptr event);

  /**
   * @brief 写debug级别日志
   * @param[in] event 日志事件
   */ 
  void debug(LogEvent::ptr event);

  /**
   * @brief 写info级别日志
   * @param[in] event 日志事件
   */ 
  void info(LogEvent::ptr event);

  /**
   * @brief 写warn级别日志
   * @param[in] event 日志事件
   */ 
  void warn(LogEvent::ptr event);

  /**
   * @brief 写error级别日志
   * @param[in] event 日志事件
   */ 
  void error(LogEvent::ptr event);
  
  /**
   * @brief 写fatal级别日志
   * @param[in] event 日志事件
   */ 
  void fatal(LogEvent::ptr event);

  /**
   * @brief 添加日志目标
   */
  void addAppender(LogAppender::ptr appender);
    
  /**
   * @brief 删除日志目标
   */ 
  void delAppender(LogAppender::ptr appender);
 
  /**
   * @brief 清空日志目标
   */
  void clearAppenders();

  /** 
   * @brief 返回日志级别
   */
  LogLevel::Level getLogLevel() const{
     return m_level;
  }

  /**
   * @brief 设置日志级别
   */
  void setLevel(LogLevel::Level val){
    m_level = val;
  }

  /**
   * @brief 返回日志名称
   */
  const std::string & getName() const{
    return m_name;
  }

private:
  //日志名称
  std::string m_name;
  //日志级别
  LogLevel::Level m_level;
  //日志目标集合
  std::list<LogAppender::ptr> m_appenders;
};

/**
 *  @brief 输出到控制台的Appender
 */ 
class StdoutLogAppender : public LogAppender{

};

/**
 *  @brief 输出到文件的Appender
 */
class FileLogAppender : public LogAppender{

};

//日志输出格式
class LogFormat{
public:

private:
};

}


#endif
