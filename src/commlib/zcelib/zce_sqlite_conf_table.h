/*!
* @copyright  2004-2014  Apache License, Version 2.0 FULLSAIL
* @filename   zce_sqlite_conf_table.h
* @author     Sailzeng <sailerzeng@gmail.com>
* @version
* @date       Thursday, November 27, 2009
* @brief
*
*
* @details
*
*
*
* @note       AI_IIJIMA 饭岛爱
*             最近比较烦比你烦也比你烦，我梦见和饭岛爱一起晚餐；梦中的餐厅灯光太昏暗，我遍寻不著那蓝色的小药丸
*             最近有点事情多到了极点，但是也无聊到了极点，发现最近代码的名字乱七八糟的东西越来越多.
*             说了很多饭岛爱的废话。不如在代码里面纪念吧，一个充满矛盾人的，名字有爱，，被无数人无数幻想，却一生无爱，
*             被李宗盛传唱，但不被大众所知，结果在死后却上了sina网的首页，引发中国人民的对日本一个女孩怀念。
*             半推荐她的小说《柏拉图式性爱》，小说在一个侧面反映了黑暗的资本主义社会对一个小女孩身心的摧残，所以推荐大家读读，
*             最后要说的是，生命不易，希望大家珍惜。
*             想起那段把一个产品做了3遍的历史。
*/


/*
表格和索引定义，
CREATE TABLE IF NOT EXISTS config_table_1(index_1 INTEGER,index_2 INTEGER,conf_data BLOB ,last_mod_time INTEGER);
CREATE UNIQUE INDEX IF NOT EXISTS cfg_table_idx_1 ON config_table_1 (index_1,index_2)

*/

#ifndef ZCE_LIB_SQLITE_CONF_TABLE_H_
#define ZCE_LIB_SQLITE_CONF_TABLE_H_

//目前版本限制只加这一个
#if SQLITE_VERSION_NUMBER >= 3005000



/*!
* @brief      用于将一些结构转换为二进制数据，存放于数据字段中
*
* @note
*/
struct ZCELIB_EXPORT AI_IIJIMA_BINARY_DATA
{


public:

    //构造和析构函数
    AI_IIJIMA_BINARY_DATA();
    ~AI_IIJIMA_BINARY_DATA();
    //
    void clear();

#if defined ZCE_USE_PROTOBUF && ZCE_USE_PROTOBUF == 1

    ///将一个结构进行编码
    template<class T>
    int protobuf_encode(unsigned int index_1,
                        unsigned int index_2,
                        const T &info);

    ///将一个结构进行解码
    template<class T>
    int protobuf_decode(unsigned int &index_1,
                        unsigned int &index_2,
                        T &info);

#endif

public:

    //动态编解码的数据区长度
    static const int MAX_LEN_OF_AI_IIJIMA_DATA = 32 * 1024 - 1;

public:

    //索引1
    unsigned int index_1_ = 0;
    //索引2 默认为0
    unsigned int index_2_ = 0;

    //数据区长度
    int ai_data_length_ = 0;
    //动态数据取
    char ai_iijima_data_[MAX_LEN_OF_AI_IIJIMA_DATA];


};


#if defined ZCE_USE_PROTOBUF && ZCE_USE_PROTOBUF == 1

template<class T>
int AI_IIJIMA_BINARY_DATA::protobuf_decode(unsigned int &index_1,
                                           unsigned int &index_2,
                                           T &info)
{
    bool bret = info.ParseFromArray(ai_iijima_data_, ai_data_length_);

    if (false == bret)
    {
        ZCE_LOGMSG(RS_ERROR, "Class %s protobuf decode fail,ParseFromArray return false.\n", typeid(info).name());
        return -1;
    }

    return 0;
}

template<class T>
int AI_IIJIMA_BINARY_DATA::protobuf_encode(unsigned int index_1,
                                           unsigned int index_2,
                                           const T &info)
{

    if (info.IsInitialized())
    {
        ZCE_LOGMSG(RS_ERROR, "class %s protobuf encode fail, IsInitialized return false.\n",
                   typeid(info).name());
        return -1;
    }

    index_1_ = index_1;
    index_2_ = index_2;

    int protobuf_len = info.ByteSize();
    if (protobuf_len > MAX_LEN_OF_AI_IIJIMA_DATA)
    {
        ZCE_LOGMSG(RS_ERROR, "Config [%d|%d] class %s protobuf encode fail, ByteSize return %d > MAX_LEN_OF_AI_IIJIMA_DATA %d.\n",
                   index_1,
                   index_2,
                   typeid(info).name(),
                   protobuf_len);
        return -1
    }

           bool bret = info.SerializeToArray(ai_iijima_data_, MAX_LEN_OF_AI_IIJIMA_DATA);
    if (bret)
    {
        ZCE_LOGMSG(RS_ERROR, "Config [%d|%d] class %s protobuf encode fail, SerializeToArray return false.\n",
                   index_1,
                   index_2,
                   typeid(info).name());
        return -1;
    }
    ai_data_length_ = protobuf_len;
    return 0;
}

#endif

typedef std::vector <AI_IIJIMA_BINARY_DATA>   ARRARY_OF_AI_IIJIMA_BINARY;



/******************************************************************************************
struct BaobaoGeneralDBConf 一个很通用的从DB中间得到通用配置信息的方法
******************************************************************************************/

//一个很通用的从DB中间得到通用配置信息的结构
class ZCELIB_EXPORT ZCE_General_Config_Table
{

public:

    ZCE_General_Config_Table();
    ~ZCE_General_Config_Table();

protected:

    ///创建TABLE SQL语句
    void sql_create_table(unsigned  int table_id);
    ///创建INDEX SQL语句
    void sql_create_index(unsigned  int table_id);


    //改写的SQL
    void sql_replace_one(unsigned  int table_id,
                         unsigned int index_1,
                         unsigned int index_2,
                         unsigned int last_mod_time);

    //得到选择一个确定数据的SQL
    void sql_select_one(unsigned int table_id,
                        unsigned int index_1,
                        unsigned int index_2);

    //得到删除数据的SQL
    void sql_delete_one(unsigned int table_id,
                        unsigned int index_1,
                        unsigned int index_2);


    //
    void sql_counter(unsigned int table_id,
                     unsigned int startno,
                     unsigned int numquery);

    //
    void sql_select_array(unsigned int table_id,
                          unsigned int startno,
                          unsigned int numquery);
    
public:

    ///创建数据表
    int create_table(unsigned int table_id);

    ///
    int replace_one(unsigned int table_id,
                    const AI_IIJIMA_BINARY_DATA &conf_data,
                    unsigned int last_mod_time);

    ///
    int select_one(unsigned int table_id,
                   AI_IIJIMA_BINARY_DATA &conf_data,
                   unsigned int &last_mod_time);

    ///
    int delete_one(unsigned int table_id,
                   unsigned int index_1,
                   unsigned int index_2);

    ///
    int counter(unsigned int table_id,
                unsigned int startno,
                unsigned int numquery,
                unsigned int &rec_count);

    ///
    int select_array(unsigned int table_id,
                     unsigned int startno,
                     unsigned int numquery,
                     ARRARY_OF_AI_IIJIMA_BINARY &ary_ai_iijma);

protected:
    //
    const static size_t MAX_SQLSTRING_LEN = 8 * 1024;

public:

    ///
    char *sql_string_ = NULL;
    ///
    ZCE_SQLite_DB_Handler *sqlite_handler_;


};


#endif //SQLITE_VERSION_NUMBER >= 3005000

#endif //ZCE_LIB_SQLITE_CONF_TABLE_H_
