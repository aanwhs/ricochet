#include "ChatMessageCommand.h"
#include "CommandDataParser.h"
#include <QDateTime>
#include <QBuffer>

REGISTER_COMMAND_HANDLER(0x10, ChatMessageCommand)

ChatMessageCommand::ChatMessageCommand(QObject *parent)
	: ProtocolCommand(parent)
{
}

void ChatMessageCommand::send(ProtocolManager *to, const QDateTime &timestamp, const QString &text)
{
	int p = prepareCommand(0x00, 1024);
	CommandDataParser builder(&commandBuffer, CommandDataParser::Write);

	builder << (quint32)timestamp.secsTo(QDateTime::currentDateTime());
	builder << text;

	sendCommand(to, true);
}

void ChatMessageCommand::process(CommandHandler &command)
{
	quint32 timestamp;
	QString text;

	qDebug() << "command data:" << command.data.toHex();

	CommandDataParser parser(const_cast<QByteArray*>(&command.data), CommandDataParser::Read);
	parser >> timestamp >> text;

	qDebug() << "Received chat message (time delta" << timestamp << "):" << text;
}

void ChatMessageCommand::processReply(quint8 state, const uchar *data, unsigned dataSize)
{
	Q_UNUSED(state);
	Q_UNUSED(data);
	Q_UNUSED(dataSize);
}
