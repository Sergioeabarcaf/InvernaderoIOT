import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';

import { ChartsModule } from 'ng2-charts';

// https://github.com/ashish-chopra/ngx-gauge
import { NgxGaugeModule } from 'ngx-gauge';

import { APP_ROUTING } from './app.routes';

import { FirebaseService } from './service/firebase.service';

import { AppComponent } from './app.component';
import { HomeComponent } from './components/home/home.component';
import { NavbarComponent } from './components/navbar/navbar.component';
import { FooterComponent } from './components/footer/footer.component';
import { HistoryComponent } from './components/history/history.component';
import { FilterComponent } from './components/filter/filter.component';
import { Last24Component } from './components/last24/last24.component';


@NgModule({
  declarations: [
    HomeComponent,
    AppComponent,
    NavbarComponent,
    FooterComponent,
    HistoryComponent,
    FilterComponent,
    Last24Component
  ],
  imports: [
    APP_ROUTING,
    BrowserModule,
    FormsModule,
    HttpClientModule,
    ChartsModule,
    NgxGaugeModule
  ],
  providers: [FirebaseService],
  bootstrap: [AppComponent]
})
export class AppModule { }
