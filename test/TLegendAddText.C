{
TLegend* leg = new TLegend(0.4,0.6,0.89,0.89);
std::cout<<"1 "<<endl;
leg->AddEntry((TObject*)0,"The Data","");
leg->AddEntry((TObject*)0, "", "");  
    leg->AddEntry((TObject*)0, "Some text", "");
    leg->AddEntry((TObject*)0, "", ""); 
std::cout<<"2 "<<endl;
// and add a header (or "title") for the legend
leg->SetHeader("The Legend Title");
leg->Draw();
}
