// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Application/QuickFAST.h>
#include <iostream>

int main()
{
  QuickFAST::Codecs::TemplateRegistryPtr registry(
    new QuickFAST::Codecs::TemplateRegistry);
  std::cout << QuickFAST::QuickFAST_Product;
  std::cout << "templates: " << registry->size() << std::endl;
  return 0;
}
