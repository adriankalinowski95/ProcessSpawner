import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ElementListContentComponent } from './element-list-content.component';

describe('ElementListContentComponent', () => {
  let component: ElementListContentComponent;
  let fixture: ComponentFixture<ElementListContentComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [ElementListContentComponent]
    })
    .compileComponents();

    fixture = TestBed.createComponent(ElementListContentComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
