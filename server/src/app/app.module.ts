import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { APP_ROUTING } from './app.routes';

import { AppComponent } from './app.component';
import { HomeComponent } from './components/home/home.component';


@NgModule({
  declarations: [
    HomeComponent,
    AppComponent
  ],
  imports: [
    APP_ROUTING,
    BrowserModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
