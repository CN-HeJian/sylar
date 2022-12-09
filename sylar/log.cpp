#include "log.h"
#include <iostream>


namespace sylar{

  const char* LogLevel::ToString(LogLevel::Level level){
    switch(level){
#define XX(name) \
      case Logger::name:\
        return #name;\
        break;
     
      XX(DEBUG)
      XX(INFO)
      XX(WARN)
      XX(ERROR)
      XX(FATAL)

#undef XX
      default:
        return "UNKNOW";
    }
    return "UNKNOW";
  }

  LogLevel::Level LogLevel::FromString(const std::string& str){
#define XX(level,v)\
    if(str == #v){\
      return LogLevel::Level::level;\
    }

    XX(DEBUG, debug)
    XX(INFO, info)
    XX(WARN, warn)
    XX(ERROR, error)
    XX(FATAL, fatal)

    XX(DEBUG, DEBUG)
    XX(INFO, INFO)
    XX(WARN,  WARN)
    XX(ERROR, ERROR)
    XX(FATAL, FATAL)

#undef XX
    return LogLevel::Level::UNKNOW;
  }

  Logger::Logger(const std::string& name)
    :m_name(name){
     
  }

  void Logger::setFormatter(std::shared_ptr<LogFormat> val){
    m_formatter = val;
    for(auto& i : m_appenders){
      i->m_formatter = m_formatter;
    }
  }
 
  void Logger::setFormatter(const std::string & val){
    sylar::LogFormat::ptr new_val(new sylar::LogFormat(val));
    setFormatter(new_val);
  }

  std::shared_ptr<LogFormat> Logger::getFormatter(){
    return m_formatter; 
  }

  void Logger::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
      auto self = shared_from_this();
      for(auto &i :m_appenders){
        i->log(self, level, event);
      }
    }
  }

  void Logger::debug(LogEvent::ptr event){
    log(LogLevel::Level::DEBUG,event);
  }

  void Logger::info(LogEvent::ptr event){
    log(LogLevel::Level::INFO,event);
  }

  void Logger::warn(LogEvent::ptr event){
    log(LogLevel::Level::WARN,event);
  }


  void Logger::error(LogEvent::ptr event){
    log(LogLevel::Level::ERROE,event);
  }

  void Logger::fatal(LogEvent::ptr event){
    log(LogLevel::Level::FATAL,event);
  }

  void Logger::addAppender(LogAppender::ptr appender){
    m_appenders.push_back(appender);
  }

  void Logger::delAppender(LogAppender::ptr appender){
    for(auto it = m_appenders.begin(); it != m_appenders.end(); it++){
      if(*it == appender){
         m_appenders.erase(it);
         break;
      }
    }
  }

  void Logger::clearAppenders(){
    m_appenders.clear();
  }
  
  LogFormat::LogFormat(const std::string& pattern)
            :m_pattern(pattern){
     init();
  }

  std::string LogFormat::format(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event){
    std::stringstream ss;
    for(auto& i:m_items){
      i->format(ss, logger, level, event);
    }
    return ss.str();
  }

  std::ostream& LogFormat::format(std::ostream& ofs, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event){
    for(auto& i:m_items){
      i->format(ofs, logger, level, event);
    }
    return ofs;
  }

  void LogFormat::init(){

  }

  void StdoutLogAppender::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
      m_formatter->format(std::cout, logger, level, event);
    }
  }

  FileLogAppender::FileLogAppender(const std::string & filename)
                  :m_filename(filename){
    reopen();
  }

  void FileLogAppender::log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event){
     if(level >= m_level){
       if(!m_formatter->format(m_filestream, logger, level, event)){
         std::cout<<"error"<<std::endl;
       }
     }
  }

  bool sylar::FileLogAppender::reopen(){
     if(m_filestream){
       m_filestream.close();
     }
     m_filestream.open(m_filename);
     return !!m_filestream;
  }


}
