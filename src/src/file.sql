-- MySQL dump 10.13  Distrib 5.7.11, for Linux (i686)
--
-- Host: localhost    Database: hotel_system_lrh
-- ------------------------------------------------------
-- Server version	5.7.11-0ubuntu6

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `consum`
--

DROP TABLE IF EXISTS `consum`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `consum` (
  `num` int(11) NOT NULL,
  `table_num` int(11) NOT NULL,
  `dishes_num` int(11) NOT NULL,
  `state` varchar(15) NOT NULL,
  `consum` float(7,1) NOT NULL,
  `submit_time` time NOT NULL,
  `pay_time` time NOT NULL DEFAULT '-01:00:00',
  `Date` date NOT NULL,
  `state1` varchar(15) NOT NULL,
  PRIMARY KEY (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `consum`
--

LOCK TABLES `consum` WRITE;
/*!40000 ALTER TABLE `consum` DISABLE KEYS */;
INSERT INTO `consum` VALUES (7001,3,4,'完成',60.0,'19:18:01','19:58:17','2017-06-26','结帐'),(7002,4,4,'完成',190.0,'19:25:19','19:57:42','2017-06-26','结帐'),(7003,5,4,'完成',12.0,'19:28:13','08:52:09','2017-06-26','结帐'),(7004,6,5,'完成',154.0,'19:38:41','-01:00:00','2017-06-26','未结帐');
/*!40000 ALTER TABLE `consum` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `consum_dish`
--

DROP TABLE IF EXISTS `consum_dish`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `consum_dish` (
  `num` int(11) NOT NULL,
  `class` varchar(20) NOT NULL,
  `name` varchar(30) NOT NULL,
  `price` float(4,1) NOT NULL,
  `start_time` time NOT NULL,
  `end_time` time NOT NULL,
  `con_num` int(11) NOT NULL,
  `dish_num` int(11) DEFAULT NULL,
  `state` varchar(20) NOT NULL,
  KEY `con_num` (`con_num`),
  CONSTRAINT `consum_dish_ibfk_1` FOREIGN KEY (`con_num`) REFERENCES `consum` (`num`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `consum_dish`
--

LOCK TABLES `consum_dish` WRITE;
/*!40000 ALTER TABLE `consum_dish` DISABLE KEYS */;
INSERT INTO `consum_dish` VALUES (5013,'凉  菜','凉拌木耳',22.0,'19:51:40','19:54:18',7001,1,'完成'),(5004,'素  菜','椒盐玉米',26.0,'19:52:14','19:54:32',7001,1,'完成'),(5006,'素  菜','酸辣土豆丝',28.0,'19:52:32','19:54:42',7001,1,'完成'),(5000,'荤  菜','驴肉火烧',35.0,'19:52:47','19:55:26',7001,1,'完成'),(5004,'素  菜','椒盐玉米',26.0,'19:53:03','19:54:54',7002,1,'完成'),(5018,'干  锅','干锅肥肠',38.0,'19:53:15','19:55:02',7002,2,'完成'),(5022,'铁  板','铁板鱿鱼',50.0,'19:53:39','19:55:09',7002,1,'完成'),(5021,'铁  板','铁板牛肉',38.0,'19:53:59','19:55:16',7002,1,'完成'),(5013,'凉  菜','凉拌木耳',22.0,'08:45:02','08:46:08',7003,1,'完成'),(5006,'素  菜','酸辣土豆丝',28.0,'08:45:14','08:46:33',7003,1,'完成'),(5028,'汤  类','山药排骨汤',28.0,'08:45:38','08:46:17',7003,1,'完成'),(5022,'铁  板','铁板鱿鱼',50.0,'08:45:49','08:46:26',7003,1,'完成'),(5013,'凉  菜','凉拌木耳',22.0,'08:47:01','08:48:42',7004,1,'完成'),(5004,'素  菜','椒盐玉米',26.0,'08:47:42','08:48:50',7004,1,'完成'),(5028,'汤  类','山药排骨汤',28.0,'08:47:56','08:48:58',7004,1,'完成'),(5022,'铁  板','铁板鱿鱼',50.0,'08:48:09','08:49:03',7004,1,'完成'),(5024,'招牌菜','糖醋里脊',28.0,'08:48:21','08:49:08',7004,1,'完成');
/*!40000 ALTER TABLE `consum_dish` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dishes`
--

DROP TABLE IF EXISTS `dishes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dishes` (
  `num` int(11) NOT NULL,
  `class` varchar(20) NOT NULL,
  `name` varchar(30) NOT NULL,
  `price` float(3,1) NOT NULL,
  PRIMARY KEY (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dishes`
--

LOCK TABLES `dishes` WRITE;
/*!40000 ALTER TABLE `dishes` DISABLE KEYS */;
INSERT INTO `dishes` VALUES (5000,'荤  菜','驴肉火烧',35.0),(5001,'荤  菜','手扒羊肉',43.0),(5002,'荤  菜','清蒸白鱼',38.0),(5003,'荤  菜','水煮肉片',33.0),(5004,'素  菜','椒盐玉米',26.0),(5005,'素  菜','麻婆豆腐',28.0),(5006,'素  菜','酸辣土豆丝',28.0),(5007,'素  菜','东坡豆腐',29.0),(5008,'荤  菜','鱼香肉丝',30.0),(5009,'素  菜','清炒黄豆芽',18.0),(5010,'素  菜','青椒炒鸡蛋',22.0),(5011,'素  菜','韭菜鸡蛋',22.0),(5012,'素  菜','木耳洋葱',26.0),(5013,'凉  菜','凉拌木耳',22.0),(5014,'凉  菜','小葱拌豆腐',23.0),(5015,'凉  菜','泡椒凤爪',30.0),(5016,'凉  菜','凉拌海带丝',27.0),(5018,'干  锅','干锅肥肠',38.0),(5019,'干  锅','干锅猪耳',44.0),(5020,'干  锅','干锅鸡块',45.0),(5021,'铁  板','铁板牛肉',38.0),(5022,'铁  板','铁板鱿鱼',50.0),(5023,'招牌菜','宫保鸡丁',35.0),(5024,'招牌菜','糖醋里脊',28.0),(5025,'招牌菜','猪肉灌肠',34.0),(5026,'汤  类','冬瓜丸子汤',27.0),(5027,'汤  类','黄瓜皮蛋汤',23.0),(5028,'汤  类','山药排骨汤',28.0),(5029,'酒  水','茅 台 酒',60.0),(5030,'酒  水','青岛啤酒',4.0),(5031,'酒  水','雪花啤酒',3.0),(5032,'酒  水','可口可乐',10.0),(5033,'酒  水','果 粒 橙',12.0),(5034,'酒  水','雪    碧',12.0);
/*!40000 ALTER TABLE `dishes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `employee`
--

DROP TABLE IF EXISTS `employee`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `employee` (
  `num` int(11) NOT NULL,
  `account` varchar(10) NOT NULL,
  `passward` varchar(10) NOT NULL,
  `name` varchar(50) NOT NULL,
  `age` int(11) NOT NULL,
  `sex` char(10) NOT NULL,
  `tel` char(11) NOT NULL,
  `address` varchar(500) NOT NULL,
  `id` char(18) NOT NULL,
  `salary` float(6,1) NOT NULL,
  `position` varchar(10) NOT NULL,
  PRIMARY KEY (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `employee`
--

LOCK TABLES `employee` WRITE;
/*!40000 ALTER TABLE `employee` DISABLE KEYS */;
INSERT INTO `employee` VALUES (1000,'admin','admin','admin',0,'NULL','admin','admin','admin',0.0,'管理员'),(2000,'waiter','123456','张三',24,'男','3457','张衡路429号','23454566',2345.0,'服务员'),(3000,'chef','123456','李四',25,'女','234567','上海市浦东新区华佗路280弄','12354656',23456.0,'厨师');
/*!40000 ALTER TABLE `employee` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `marterial`
--

DROP TABLE IF EXISTS `marterial`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `marterial` (
  `num` int(11) NOT NULL,
  `name` varchar(20) NOT NULL,
  `repertory` float(6,1) NOT NULL DEFAULT '0.0',
  `price` float(3,1) NOT NULL DEFAULT '0.0',
  PRIMARY KEY (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `marterial`
--

LOCK TABLES `marterial` WRITE;
/*!40000 ALTER TABLE `marterial` DISABLE KEYS */;
INSERT INTO `marterial` VALUES (6000,'酒水',89.0,5.0),(6001,'调料',401.0,2.3),(6002,'面',162.0,3.0),(6003,'米',482.0,2.0),(6004,'土豆',160.0,3.8),(6005,'青菜',49.0,3.3),(6006,'黄瓜',109.0,2.9),(6007,'西红柿',47.0,2.7),(6008,'莴笋',107.0,3.9);
/*!40000 ALTER TABLE `marterial` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `member`
--

DROP TABLE IF EXISTS `member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `member` (
  `num` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `sex` char(10) NOT NULL,
  `age` int(11) NOT NULL,
  `tel` char(11) NOT NULL,
  `id` char(18) NOT NULL,
  `passward` varchar(10) NOT NULL,
  `integral` int(11) NOT NULL DEFAULT '100',
  PRIMARY KEY (`num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `member`
--

LOCK TABLES `member` WRITE;
/*!40000 ALTER TABLE `member` DISABLE KEYS */;
INSERT INTO `member` VALUES (1000,'张三','男',23,'13298331913','12345','123456',0),(1002,'王五','男',34,'15738897564','1245365768','123456',0),(1003,'123','123',123,'123','123','123',767),(1005,'1233','12',1233,'125','312','125',0),(1006,'131','3',31,'13','131','13',1),(1007,'赵六','男',34,'000','2345678','123',1),(1008,'h55e-','g7',23,'121','23456','121',0);
/*!40000 ALTER TABLE `member` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-07-28  9:57:31
