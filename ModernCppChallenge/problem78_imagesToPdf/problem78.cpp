
#include <iostream>
#include <string_view>
#include <filesystem>
#include <vector>

#include "PDFWriter/PDFWriter.h"
#include "PDFWriter/PDFPage.h"
#include "PDFWriter/PageContentContext.h"
#include "PDFWriter/PDFFormXObject.h"

namespace fs = std::filesystem;
std::vector<std::string> get_images(fs::path const & dirpath)
{
    std::vector<std::string> paths;

    for (auto const & p : fs::directory_iterator(dirpath))
    {
        if (p.path().extension() == ".jpg")
            paths.push_back(p.path().string());
    }
    return paths;
}

void print_pdf(fs::path const & pdfpath, fs::path const & dirpath)
{
    const int height = 842;
    const int width = 595;
    const int margin = 20;

    auto const image_paths = get_images(dirpath);

    PDFWriter pdf;
    pdf.StartPDF(pdfpath.string(), ePDFVersion13);

    PDFPage* page = nullptr;
    PageContentContext* context = nullptr;

    auto top = height - margin;
    for (size_t i = 0; i < image_paths.size(); ++i)
    {
        auto const dims = pdf.GetImageDimensions(image_paths[i]);

        if (i == 0 || top - dims.second < margin)
        {
            if (page != nullptr)
            {
                pdf.EndPageContentContext(context);
                pdf.WritePageAndRelease(page);
            }

            page = new PDFPage();
            page->SetMediaBox(PDFRectangle(0, 0, width, height));
            context = pdf.StartPageContentContext(page);

            top = height - margin;
        }

        context->DrawImage(margin, top - dims.second, image_paths[i]);

        top -= dims.second + margin;
    } 
    if (page != nullptr)
    {
        pdf.EndPageContentContext(context);
        pdf.WritePageAndRelease(page);
    }

    pdf.EndPDF();
}

int main()
{
    print_pdf("sample.pdf", "res");
}