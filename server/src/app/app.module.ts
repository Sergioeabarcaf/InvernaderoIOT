import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { ChartsModule } from 'ng2-charts';
// https://github.com/mattlewis92/angular-gauge
import { GaugeModule } from 'angular-gauge';

import { APP_ROUTING } from './app.routes';

import { FirebaseService } from './service/firebase.service';

import { AppComponent } from './app.component';
import { HomeComponent } from './components/home/home.component';
import { NavbarComponent } from './components/navbar/navbar.component';
import { FooterComponent } from './components/footer/footer.component';
import { HistoryComponent } from './components/history/history.component';


@NgModule({
  declarations: [
    HomeComponent,
    AppComponent,
    NavbarComponent,
    FooterComponent,
    HistoryComponent
  ],
  imports: [
    APP_ROUTING,
    BrowserModule,
    FormsModule,
    HttpClientModule,
    ChartsModule,
    GaugeModule.forRoot()
  ],
  providers: [FirebaseService],
  bootstrap: [AppComponent]
})
export class AppModule { }
