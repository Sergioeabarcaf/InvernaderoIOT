import { Component } from '@angular/core';

@Component({
  selector: 'app-navbar',
  templateUrl: './navbar.component.html',
  styleUrls: ['./navbar.component.css']
})
export class NavbarComponent {

  constructor() { }

  ocultarNav(){
    var navbar = document.getElementById("navbarNav");
    navbar.classList.toggle("show");
  }

}
