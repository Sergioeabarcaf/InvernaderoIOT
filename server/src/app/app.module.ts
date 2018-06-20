import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';

import { ChartsModule } from 'ng2-charts';

import { APP_ROUTING } from './app.routes';

import { FirebaseService } from './service/firebase.service';

import { AppComponent } from './app.component';
import { HomeComponent } from './components/home/home.component';
import { DispositivosComponent } from './components/dispositivos/dispositivos.component';
import { ParametrosComponent } from './components/parametros/parametros.component';
import { NavbarComponent } from './components/navbar/navbar.component';
import { FooterComponent } from './components/footer/footer.component';


@NgModule({
  declarations: [
    HomeComponent,
    AppComponent,
    DispositivosComponent,
    ParametrosComponent,
    NavbarComponent,
    FooterComponent
  ],
  imports: [
    APP_ROUTING,
    BrowserModule,
    HttpClientModule,
    ChartsModule
  ],
  providers: [FirebaseService],
  bootstrap: [AppComponent]
})
export class AppModule { }
