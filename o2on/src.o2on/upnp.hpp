/* Copyright (C) 2006 o2on project. All rights reserved.
 * http://o2on.net/
 */

/*
 * project		: 
 * filename		: upnp.h
 * description	: 
 *
 */

#include "simplessdpsocket.hpp"
#include "simplehttpsocket.hpp"
#include "upnp_description.hpp"




class UPnP
{
protected:
	void (*MessageHandler)(const char *);
	string *LogBuffer;

public:
	UPnP(void)
		: MessageHandler(NULL)
		, LogBuffer(NULL)
	{
	}
	~UPnP()
	{
	}
	void SetMessageHandler(void (*func)(const char *))
	{
		MessageHandler = func;
	}
	void SetLogBuffer(string *buffer)
	{
		LogBuffer = buffer;
	}

public:
	// -----------------------------------------------------------------------
	//	SearchIGDs
	// -----------------------------------------------------------------------
	size_t SearchIGDs(UPnPObjectList &objectList)
	{
		if (MessageHandler)
			MessageHandler("Internet Gateway Deviceを探しています...");

		SSDPSocket ssdp(3000);
		ssdp.setlogbuffer(LogBuffer);
		ushort bind_port = 1024;
		while (!ssdp.initialize(bind_port) && bind_port < 65535) {
			bind_port++;
		}
		if (bind_port == 65535) {
			if (MessageHandler) {
				MessageHandler("ERROR: bind失敗");
			}
			return (0);
		}

		objectList.clear();

		strarray locations;
		ssdp.discover("urn:schemas-upnp-org:device:InternetGatewayDevice:1", locations);

		for (size_t i = 0; i < locations.size(); i++) {
			UPnPObject obj;
			obj.location = locations[i];
			objectList.push_back(obj);

			if (MessageHandler) {
				string msg;
				msg = "発見: ";
				msg += locations[i];
				MessageHandler(msg.c_str());
			}
		}
		return (locations.size());
	}

	// -----------------------------------------------------------------------
	//	GetDeviceDescriptions
	// -----------------------------------------------------------------------
	size_t GetDeviceDescriptions(UPnPObjectList &objectList)
	{
		if (MessageHandler)
			MessageHandler("デバイスの詳細を取得しています...");

		if (objectList.empty()) {
			if (MessageHandler) {
				MessageHandler("ERROR: 存在しません");
			}
			return false;
		}

		size_t done = 0;
		for (size_t i = 0; i < objectList.size(); i++) {
			if (GetDeviceDescription(objectList[i]))
				done++;
		}

		if (done == 0) {
			if (MessageHandler)
				MessageHandler("ERROR: 全て失敗しました");
			return (0);
		}

		return (done);
	}

	// -----------------------------------------------------------------------
	//	GetDeviceDescription
	// -----------------------------------------------------------------------
	bool GetDeviceDescription(UPnPObject &object)
	{
		if (MessageHandler) {
			string msg;
			msg = "取得中: ";
			msg += object.location;
			MessageHandler(msg.c_str());
		}

		HTTPSocket hsock(3000, "");
		hsock.setlogbuffer(LogBuffer);
		if (!hsock.request(object.location.c_str(), NULL, 0, true)) {
			if (MessageHandler)
				MessageHandler("ERROR: 接続失敗");
			return false;
		}

		HTTPHeader hdr(HTTPHEADERTYPE_RESPONSE);
		string buff;
		int ret;
		while ((ret = hsock.response(buff, hdr)) >= 0) {
			if (hdr.contentlength) {
				if (buff.size() - hdr.length >= hdr.contentlength)
					break;
			}
		}
		if (LogBuffer) {
			*LogBuffer += LINESTR;
			*LogBuffer += buff;
		}
		/*
		if (hdr.contentlength == 0 || buff.size() - hdr.length < hdr.contentlength) {
			if (MessageHandler)
				MessageHandler("ERROR: 取得失敗");
			return false;
		}
		*/
		UPnPDeviceDescriptionParser parser(&object, MessageHandler);
		if (!parser.Parse(L"us-ascii", &buff[hdr.length], buff.size() - hdr.length))
			return false;

		return true;
	}

	// -----------------------------------------------------------------------
	//	GetServiceDescriptions
	// -----------------------------------------------------------------------
	size_t GetServiceDescriptions(UPnPServiceList &serviceList)
	{
		if (MessageHandler)
			MessageHandler("サービスの詳細を取得しています...");

		if (serviceList.empty()) {
			if (MessageHandler) {
				MessageHandler("ERROR: 存在しません");
			}
			return false;
		}

		size_t done = 0;
		for (size_t i = 0; i < serviceList.size(); i++) {
			if (GetServiceDescription(serviceList[i]))
				done++;
		}

		if (done == 0) {
			if (MessageHandler)
				MessageHandler("ERROR: 全て失敗しました");
			return (0);
		}

		return (done);
	}

	// -----------------------------------------------------------------------
	//	GetServiceDescription
	// -----------------------------------------------------------------------
	bool GetServiceDescription(UPnPService &service)
	{
		if (MessageHandler) {
			string msg;
			msg = "取得中: ";
			msg += service.SCPDURL;
			MessageHandler(msg.c_str());
		}

		HTTPSocket hsock(3000, "");
		hsock.setlogbuffer(LogBuffer);
		if (!hsock.request(service.SCPDURL.c_str(), NULL, 0, true)) {
			if (MessageHandler)
				MessageHandler("ERROR: 接続失敗");
			return false;
		}

		HTTPHeader hdr(HTTPHEADERTYPE_RESPONSE);
		string buff;
		int ret;
		while ((ret = hsock.response(buff, hdr)) >= 0) {
			if (hdr.contentlength) {
				if (buff.size() - hdr.length >= hdr.contentlength)
					break;
			}
		}
		if (LogBuffer) {
			*LogBuffer += LINESTR;
			*LogBuffer += buff;
		}
		/*
		if (hdr.contentlength == 0 || buff.size() - hdr.length < hdr.contentlength) {
			if (MessageHandler)
				MessageHandler("ERROR: 取得失敗");
			return false;
		}
		*/
		UPnPServiceDescriptionParser parser(&service, MessageHandler);
		if (!parser.Parse(L"us-ascii", &buff[hdr.length], buff.size() - hdr.length))
			return false;

		return true;
	}

	// -----------------------------------------------------------------------
	//	DoServiceActions
	// -----------------------------------------------------------------------
	size_t DoServiceActions(UPnPServiceList &serviceList, const char *actName)
	{
		string msg;

		if (MessageHandler) {
			msg = actName;
			msg += "を実行しています";
			MessageHandler(msg.c_str());
		}

		size_t count = 0;
		size_t done = 0;
		for (size_t i = 0; i < serviceList.size(); i++) {
			if (serviceList[i].getAction(actName)) {
				count++;
				if (DoServiceAction(serviceList[i], actName))
					done++;
			}
		}

		if (count == 0) {
			if (MessageHandler) {
				msg = "ERROR: ";
				msg = actName;
				msg += "を実行可能なサービスが存在しません";
				MessageHandler(msg.c_str());
			}
			return (0);
		}

		if (done == 0) {
			if (MessageHandler)
				MessageHandler("ERROR: 全て失敗しました");
			return (0);
		}

		return (done);
	}

	// -----------------------------------------------------------------------
	//	DoServiceAction
	// -----------------------------------------------------------------------
	bool DoServiceAction(UPnPService &service, const char *actName)
	{
		string msg;

		if (MessageHandler) {
			msg = "実行中: ";
			msg += service.serviceId;
			MessageHandler(msg.c_str());
		}

		UPnPAction *action;
		if ((action = service.getAction(actName)) == NULL) {
			msg = "ERROR: アクション";
			msg += actName;
			msg += "は実行できません";
			MessageHandler(msg.c_str());
		}

		string body;
		body = "<?xml version=\"1.0\"?>\r\n";
		body += "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n";
		body += "<SOAP-ENV:Body>\r\n";
		body += "<m:";
		body += actName;
		body += " xmlns:m=\"";
		body += service.serviceType;
		body += "\">\r\n";
		for (UPnPArgumentListIt it = action->argumentList.begin(); it != action->argumentList.end(); it++) {
			if (it->direction == "in") {
				body += "<";
				body += it->name;
				body += ">";
				body += it->value;
				body += "</";
				body += it->name;
				body += ">\r\n";
			}
		}
		body += "</m:";
		body += actName;
		body += ">\r\n";
		body += "</SOAP-ENV:Body>\r\n";
		body += "</SOAP-ENV:Envelope>\r\n";

		HTTPSocket hsock(3000, "");
		hsock.setlogbuffer(LogBuffer);
		HTTPHeader hdr(HTTPHEADERTYPE_REQUEST);
		hdr.method = "POST";
		msg = service.serviceType;
		msg += "#";
		msg += actName;
		hdr.AddFieldString("SoapAction", msg.c_str());
		hdr.AddFieldString("Content-Type", "text/xml");

		if (!hsock.request(service.controlURL.c_str(), hdr, body.c_str(), body.size(), true)) {
			if (MessageHandler)
				MessageHandler("ERROR: 接続失敗");
			return false;
		}

		HTTPHeader rhdr(HTTPHEADERTYPE_RESPONSE);
		string buff;
		int ret;
		while ((ret = hsock.response(buff, rhdr)) >= 0) {
			if (rhdr.contentlength) {
				if (buff.size() - rhdr.length >= rhdr.contentlength)
					break;
			}
		}
		if (LogBuffer) {
			*LogBuffer += LINESTR;
			*LogBuffer += buff;
		}
		if (rhdr.status != 200 /*rhdr.contentlength == 0 || buff.size() - rhdr.length < rhdr.contentlength*/) {
			if (MessageHandler)
				MessageHandler("ERROR: 取得失敗");
			return false;
		}

		UPnPSOAPResponseParser parser(&service, MessageHandler);
		if (!parser.Parse(L"us-ascii", &buff[rhdr.length], buff.size() - rhdr.length))
			return false;

		if (MessageHandler)
			MessageHandler("成功");

		return true;
	}

};
