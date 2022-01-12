/** License: zlib
 * Copyright (c) 2015 Fabian Vogt
 *
 * This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution. */

#include <cstdint>
#include <iostream>

#include <QFileInfo>
#include <QCoreApplication>
#include <QImage>
#include <QStringList>
#include <QCommandLineParser>
#include <QFile>

#define SWAP_WORD(x) (x)


constexpr uint16_t toRGB16(QRgb rgb)
{
	return SWAP_WORD((((qRed(rgb) >> 3) & 0x1F) << 11) | (((qGreen(rgb) >> 3) & 0x1F) << 6) |
                       (((qBlue(rgb) >> 3) & 0x1F) << 1) | (255 >> 7));
    //return (qRed(rgb) & 0b11111) << 0 | (qGreen(rgb) & 0b11111) << 5 | (qBlue(rgb) & 0b11111) >> 10;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName("ConvertImg");
    QCoreApplication::setApplicationVersion("0.9");

    //Parse the commandline
    QCommandLineParser parser;

    parser.setApplicationDescription("Convert pictures and images to .h files suitable for various libraries");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("image", "The image to be converted");
    parser.addPositionalArgument("output", "Write to this file instead of stdout (optional)");

    QCommandLineOption opt_format("format", "The format to convert to. Available options: nsdl,ngl,ngl2d,n2dlib,python", "format");
    QCommandLineOption opt_var_name("var", "The name of the global array", "name");
    QCommandLineOption opt_notstatic("not-static", "Don't make the variables static");
    parser.addOption(opt_format);
    parser.addOption(opt_var_name);
    parser.addOption(opt_notstatic);

    parser.process(app);

    //Verify the arguments
    if(!parser.isSet(opt_format))
    {
        std::cerr << "You have to set the output format!" << std::endl;
        return 1;
    }
    QString format = parser.value(opt_format);
    if(format != "nsdl" && format != "ngl" && format != "ngl2d" && format != "n2dlib" && format != "python")
    {
        std::cerr << "Only nsdl, ngl, ngl2d, n2dlib or python as format allowed!" << std::endl;
        return 1;
    }

    if(parser.positionalArguments().size() == 0)
    {
        std::cerr << "No input file provided!" << std::endl;
        return 1;
    }

    //Finally do stuff: Load the image and convert it to RGB8565
    QFileInfo file_info(parser.positionalArguments()[0]);
    QString var_name = parser.isSet(opt_var_name) ? parser.value(opt_var_name) : file_info.baseName();
    bool notstatic = parser.isSet(opt_notstatic);

    QImage i = QImage(parser.positionalArguments()[0]);
    if(i.isNull())
    {
        std::cerr << "Loading of image '" << parser.positionalArguments()[0].toStdString() << "' failed!" << std::endl;
        return 1;
    }

    //To have the best transparency we need to find a color that is not present in the image
    bool has_transparency = i.hasAlphaChannel();
    i = i.convertToFormat(QImage::Format_ARGB32);

    uint16_t unused_color = 0x0000;
    if(has_transparency || format == "n2dlib")
    {
        bool color_present[0x10000]{false};

        //nGL needs a transparent_color of 0x0 for nglDrawTriangle.
        //Replace black by dark grey
        if(format == "ngl")
        {
            for(unsigned int y = 0; y < static_cast<unsigned int>(i.height()); ++y)
            {
                QRgb *scanline = reinterpret_cast<QRgb*>(i.scanLine(y));
                for(unsigned int x = 0; x < static_cast<unsigned int>(i.width()); ++x, ++scanline)
                {
                    if(qAlpha(*scanline) >= 0x80)
                    {
                        uint16_t color = toRGB16(*scanline);
                        if(color == 0x0)
                            *scanline = qRgb(0b1000, 0b1000, 0b1000);

                        color_present[toRGB16(*scanline)] = true;
                    }
                }
            }
        }
        else
        {
            for(unsigned int y = 0; y < static_cast<unsigned int>(i.height()); ++y)
            {
                QRgb *scanline = reinterpret_cast<QRgb*>(i.scanLine(y));
                for(unsigned int x = 0; x < static_cast<unsigned int>(i.width()); ++x, ++scanline)
                {
                    if(qAlpha(*scanline) >= 0x80)
                        color_present[toRGB16(*scanline)] = true;
                }
            }
        }

        auto i = std::find(color_present, color_present + 0x10000, false);
        if(i == color_present + 0x10000)
        {
            std::cerr << "Every possible color is present in this image! Transparency not available!" << std::endl;
            return 1;
        }
        else
            unused_color = i - color_present;
    }

    QStringList lines;

    if(format == "python")
    {
        QString transparency = "None";
        if(has_transparency)
            transparency = QString("0x%0").arg(unused_color, 0, 16);

        lines << ("# Generated from " + file_info.fileName());
        lines << "from nsp import Texture";
        lines << QString("%0 = Texture(%1, %2, %3)").arg(var_name).arg(i.width()).arg(i.height()).arg(transparency);

        QByteArray ba;

        for(unsigned int y = 0; y < static_cast<unsigned int>(i.height()); ++y)
        {
            QRgb *scanline = reinterpret_cast<QRgb*>(i.scanLine(y));
            for(unsigned int x = 0; x < static_cast<unsigned int>(i.width()); ++x, ++scanline)
            {
                uint16_t color = (qAlpha(*scanline) >= 0x80) ? toRGB16(*scanline) : unused_color;
                ba.append(color & 0xFF);
                ba.append(color >> 8);
            }
        }

        lines << QString("%0.setData('%1')").arg(var_name).arg(QString::fromLocal8Bit(ba.toBase64()));
    }
    else
    {
        lines << ("//Generated from " + file_info.fileName() + " (output format: " + format + ")");

        QString str_static = "static ";
        if(notstatic)
            str_static = "";

        if(format == "ngl" || format == "ngl2d")
            lines << QString("%0uint16_t %1_data[] = {").arg(str_static).arg(var_name);
        else if(format == "nsdl")
            lines << QString("%0uint16_t %1[] = {0x2a01,\n%2,\n%3,\n0x0000,").arg(str_static).arg(var_name).arg(i.width()).arg(i.height());
        else if(format == "n2dlib")
            lines << QString("%0uint16_t %1[] = {%2,\n%3,\n0x%4,").arg(str_static).arg(var_name).arg(i.width()).arg(i.height()).arg(unused_color, 0, 16);

        for(unsigned int y = 0; y < static_cast<unsigned int>(i.height()); ++y)
        {
            QRgb *scanline = reinterpret_cast<QRgb*>(i.scanLine(y));
            QString line;
            for(unsigned int x = 0; x < static_cast<unsigned int>(i.width()); ++x, ++scanline)
            {
                uint16_t color = (qAlpha(*scanline) >= 0x80) ? toRGB16(*scanline) : unused_color;
                line += QString("0x%0, ").arg(color, 0, 16);
            }
            lines << line;
        }

        lines << QString("};");

        if(format == "ngl" || format == "ngl2d")
        {
            lines << str_static + QString("TEXTURE ") + var_name + "{";
            lines << QString(".width = %0,").arg(i.width());
            lines << QString(".height = %0,").arg(i.height());
            lines << QString(".has_transparency = %0,").arg(has_transparency ? "true" : "false");
            lines << QString(".transparent_color = %0,").arg(unused_color);
            lines << QString(".bitmap = %0_data };").arg(var_name);
        }
    }

    if(parser.positionalArguments().size() >= 2)
    {
        QFile output(parser.positionalArguments()[1]);
        output.open(QFile::WriteOnly);
        output.write(lines.join("\n").toUtf8());
    }
    else
        std::cout << lines.join("\n").toStdString();
    
    app.quit();

    return 0;
}
